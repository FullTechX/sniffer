#pragma once
#include <string>
#include <atomic>
#include <thread>

/// เริ่ม thread เขียน log (เรียกก่อน pcap_loop)
void start_logger_thread();
/// แจ้งให้ thread หยุดทำงาน (หลัง pcap_loop)
void stop_logger_thread();
/// push ข้อความ log เข้า queue
void enqueue_log(const std::string &entry);
