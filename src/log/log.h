#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <memory>
#include <string>

enum LogType {
    CLOG_INFO,
    CLOG_WARNING,
    CLOG_ERROR
};

struct LogEntry {
    LogType type;
    std::string message;
};

class LogSink {
public:
    virtual ~LogSink() {}
    virtual void Sink(const LogEntry& entry) = 0; // Pure virtual function
};

class Log {
    public:
        static std::vector<LogEntry> messages;
        static std::vector<std::shared_ptr<LogSink>> sinks;
        static void AddSink(std::shared_ptr<LogSink> sink);

        static void Inf(const char* format, ...);
        static void Warn(const char* format, ...);
        static void Err(const char* format, ...);
};

#endif
