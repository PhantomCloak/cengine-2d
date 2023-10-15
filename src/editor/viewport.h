#pragma once
#include "editor_window.h"
#include "glm/glm.hpp"

class EditorViewPort : public EditorWindow {
  public:
    void RenderWindow() override;
    bool IsFocused() override;
    glm::vec2 ViewportCursorPos;
    glm::vec2 ViewportSize;
  private:
    bool isFocused;
};
