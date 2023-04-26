#include "log.h"
#include <chrono>
#include <iostream>


std::vector<LogEntry> Log::messages;

using sysclock_t = std::chrono::system_clock;

const std::string getDateString() {
    std::time_t now = sysclock_t::to_time_t(sysclock_t::now());
    char buf[16] = { 0 };
    std::strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

void Log::Inf(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = CLOG_INFO;
    logEntry.message = "(INF): [" + getDateString() + "]: " + message;
    std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}

void Log::Warn(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = CLOG_INFO;
    logEntry.message = "(WARN): [" + getDateString() + "]: " + message;
    std::cout << "\x1B[33m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}

void Log::Err(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = CLOG_ERROR;
    logEntry.message = "(ERR): [" + getDateString() + "]: " + message;
    messages.push_back(logEntry);
    std::cerr << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;
}
