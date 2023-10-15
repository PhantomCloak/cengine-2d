#include "editor_sink.h"

std::function<void(const LogEntry& entry)> sinkCallback;

void EditorSink::AttachSinkCallback(std::function<void(const LogEntry& entry)>&& callback) {
    sinkCallback = callback;
}

void EditorSink::Sink(const LogEntry& entry) {
  sinkCallback(entry);
}
