#include "dns_cache.h"
#include <arpa/inet.h>
#include <netdb.h>

static std::unordered_map<uint32_t, std::string> dns_cache;
static std::mutex dns_mutex;

/// ถ้า cache เต็ม จะเก็บแค่ 1024 entry แรก
std::string ip_to_hostname_cached(uint32_t ip_be) {
    {
        std::lock_guard<std::mutex> lock(dns_mutex);
        auto it = dns_cache.find(ip_be);
        if (it != dns_cache.end()) return it->second;
    }

    struct in_addr addr{ ip_be };
    struct hostent* he = gethostbyaddr(&addr, sizeof(addr), AF_INET);

    std::string name = he ? he->h_name : inet_ntoa(addr);

    {
        std::lock_guard<std::mutex> lock(dns_mutex);
        if (dns_cache.size() < 1024) {
            dns_cache[ip_be] = name;
        }
    }
    return name;
}
