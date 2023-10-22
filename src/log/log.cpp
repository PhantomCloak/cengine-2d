#include "log.h"
#include <cstdarg>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <memory>

std::vector<LogEntry> Log::messages;
std::vector<std::shared_ptr<LogSink>> Log::sinks;

using sysclock_t = std::chrono::system_clock;


std::string StringFormat(const char* format, va_list args) {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    return std::string(buffer);
}

const std::string getDateString() {
    std::time_t now = sysclock_t::to_time_t(sysclock_t::now());
    char buf[16] = { 0 };
    std::strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
};

void Log::AddSink(std::shared_ptr<LogSink> sink) {
    sinks.push_back(sink);
}

void Log::Inf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::string message = StringFormat(format, args);
    va_end(args);
    LogEntry logEntry;
    logEntry.type = CLOG_INFO;
    logEntry.message = "(INF): [" + getDateString() + "]: " + message;
    std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
    for (const auto& sink : sinks) {
        sink->Sink(logEntry);
    }
}

void Log::Warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::string message = StringFormat(format, args);
    va_end(args);
   LogEntry logEntry;
    logEntry.type = CLOG_WARNING;
    logEntry.message = "(WARN): [" + getDateString() + "]: " + message;
    std::cout << "\x1B[33m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
    for (const auto& sink : sinks) {
        sink->Sink(logEntry);
    }
}

void Log::Err(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::string message = StringFormat(format, args);
    va_end(args);
   LogEntry logEntry;
    logEntry.type = CLOG_ERROR;
    logEntry.message = "(ERR): [" + getDateString() + "]: " + message;
    std::cerr << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
    for (const auto& sink : sinks) {
        sink->Sink(logEntry);
    }
}

