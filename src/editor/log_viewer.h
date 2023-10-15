#pragma once
#include "editor_window.h"
#include "../log/log.h"
#include "log_sinks/editor_sink.h"

class LogView : public EditorWindow {
  public:
    LogView(std::shared_ptr<LogSink> editorSink = NULL);
    bool IsFocused() override;
    void RenderWindow() override;
  private:
    std::shared_ptr<EditorSink> editorSink;
};
