#pragma once
#include "editor_window.h"

class SystemExplorer : public EditorWindow {
  public:
    void RenderWindow() override;
    bool IsFocused() override;
};
