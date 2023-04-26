#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
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

class Log {
    public:
        static std::vector<LogEntry> messages;
        static void Inf(const std::string& message);
        static void Warn(const std::string& message);
        static void Err(const std::string& message);
};

#endif
