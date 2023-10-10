#pragma once
#include "editor_window.h"
#include "glm/glm.hpp"

class EditorViewPort : EditorWindow {
  public:
    void RenderWindow() override;
    glm::vec2 ViewportCursorPos;
    glm::vec2 ViewportSize;
};
