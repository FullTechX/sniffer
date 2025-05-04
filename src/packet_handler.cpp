#include "packet_handler.h"
#include "dns_cache.h"
#include "logger.h"
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <ctime>
#include <cstdio>

static bool is_http_request(const u_char* data, int size) {
    if (size < 4) return false;
    return (data[0]=='G'&&data[1]=='E'&&data[2]=='T'&&data[3]==' ') ||
           (data[0]=='P'&&data[1]=='O'&&data[2]=='S'&&data[3]=='T') ||
           (data[0]=='H'&&data[1]=='E'&&data[2]=='A'&&data[3]=='D');
}

static void on_packet(u_char* user, const pcap_pkthdr* h, const u_char* p) {
    // tight BPF: tcp port 80 and payload flag already applied
    if (h->caplen < sizeof(ether_header)+sizeof(iphdr)) return;

    auto eth = (const ether_header*)p;
    auto ip  = (const iphdr*)(p + sizeof(ether_header));
    if (ip->protocol != IPPROTO_TCP) return;

    int ip_len = (ip->ihl<<2);
    auto tcp = (const tcphdr*)(p + sizeof(ether_header) + ip_len);
    int tcp_len = (tcp->th_off<<2);

    int payload_size = ntohs(ip->tot_len) - ip_len - tcp_len;
    if (payload_size <= 0) return;

    const u_char* payload = p + sizeof(ether_header) + ip_len + tcp_len;
    if (!is_http_request(payload, payload_size)) return;

    // เตรียม timestamp
    struct tm *tm = localtime((time_t*)&h->ts.tv_sec);
    char tbuf[32];
    snprintf(tbuf, sizeof(tbuf),
        "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec,
        h->ts.tv_usec/1000);

    // แปลง IP & hostname
    uint32_t src_be = ip->saddr, dst_be = ip->daddr;
    char sip[INET_ADDRSTRLEN], dip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &src_be, sip, sizeof(sip));
    inet_ntop(AF_INET, &dst_be, dip, sizeof(dip));

    std::string shost = ip_to_hostname_cached(src_be);
    std::string dhost = ip_to_hostname_cached(dst_be);

    // สร้าง log entry ด้วย snprintf
    char buf[2048];
    int len = snprintf(buf, sizeof(buf),
        "[%s] From: %s (%s):%u → To: %s (%s):%u\n",
        tbuf,
        sip, shost.c_str(), ntohs(tcp->source),
        dip, dhost.c_str(), ntohs(tcp->dest)
    );

    // push ขึ้น queue
    enqueue_log(std::string(buf, len));
}

int setup_and_run(const char *iface) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(iface, 65535, 1, 1000, errbuf);
    if (!handle) {
        std::fprintf(stderr, "pcap_open_live failed: %s\n", errbuf);
        return 1;
    }

    struct bpf_program filter;
    const char* expr = "tcp port 80 and (tcp[13] & 0x18 != 0)";
    if (pcap_compile(handle, &filter, expr, 1, PCAP_NETMASK_UNKNOWN)==-1 ||
        pcap_setfilter(handle, &filter)==-1) {
        std::fprintf(stderr, "pcap_setfilter failed\n");
        return 1;
    }

    // เริ่ม loop
    pcap_loop(handle, 0, on_packet, nullptr);
    pcap_close(handle);
    return 0;
}
