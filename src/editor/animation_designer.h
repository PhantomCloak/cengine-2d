#pragma once
#include "editor_window.h"
#include "glm/glm.hpp"

class AnimationDesigner : public EditorWindow {
  public:
    void RenderWindow() override;
    bool IsFocused() override;
    void RenderNodes();
    void AddNode(std::string nodeName);
    glm::vec2 ViewportCursorPos;
    glm::vec2 ViewportSize;
  private:
    bool isFocused;
};
