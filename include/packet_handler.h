#pragma once
#include <pcap.h>

/// ตั้ง filter, เริ่มจับ packet, ส่งไป packet_handler_impl
int setup_and_run(const char *iface);
