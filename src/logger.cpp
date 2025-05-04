#include "logger.h"
#include <fstream>
#include <deque>
#include <condition_variable>
#include <mutex>

static std::deque<std::string> log_queue;
static std::mutex queue_mutex;
static std::condition_variable queue_cv;
static std::atomic<bool> done{false};
static std::thread logger_thread;

static void logger_func() {
    std::ofstream out("http_sniffer.log", std::ios::app);
    while (!done.load() || !log_queue.empty()) {
        std::unique_lock<std::mutex> lk(queue_mutex);
        queue_cv.wait(lk, []{ return done.load() || !log_queue.empty(); });
        while (!log_queue.empty()) {
            out << log_queue.front();
            log_queue.pop_front();
        }
        out.flush();
    }
}

void start_logger_thread() {
    logger_thread = std::thread(logger_func);
}

void stop_logger_thread() {
    done = true;
    queue_cv.notify_one();
    if (logger_thread.joinable()) logger_thread.join();
}

void enqueue_log(const std::string &entry) {
    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        log_queue.push_back(entry);
    }
    queue_cv.notify_one();
}

