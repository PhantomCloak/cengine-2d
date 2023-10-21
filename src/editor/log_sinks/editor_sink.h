#pragma once
#include "../../log/log.h"
#include <functional>

class EditorSink : public LogSink {
    public:
    void AttachSinkCallback(std::function<void(const LogEntry& entry)>&& callback);
    void Sink(const LogEntry& entry) override;
};
