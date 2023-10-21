#pragma once
#include "editor_window.h"
#include "glm/glm.hpp"

class AnimationEditor : public EditorWindow {
  public:
    void RenderWindow() override;
    bool IsFocused() override;
  private:
    bool isFocused;
};
