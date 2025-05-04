#include "packet_handler.h"
#include "logger.h"
#include <pcap.h>
#include <iostream>

int main() {
    // สร้าง logger thread
    start_logger_thread();

    // เลือก interface ตัวแรก
    char err[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs;
    if (pcap_findalldevs(&alldevs, err) == -1 || !alldevs) {
        std::cerr << "pcap_findalldevs failed: " << err << "\n";
        return 1;
    }
    const char* iface = alldevs->name;
    std::cout << "Listening on " << iface << "...\n";

    int ret = setup_and_run(iface);

    // หยุด logger
    stop_logger_thread();
    pcap_freealldevs(alldevs);
    return ret;
}
