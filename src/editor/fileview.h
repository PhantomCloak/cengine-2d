#pragma once
#include "editor_window.h"

class FileView : public EditorWindow {
  public:
    void RenderWindow() override;
    bool IsFocused() override;
};
