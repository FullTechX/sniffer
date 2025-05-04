#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <netinet/in.h>

/// แปลง IPv4 address (network-order uint32_t) เป็น hostname ด้วย cache
std::string ip_to_hostname_cached(uint32_t ip_be);
