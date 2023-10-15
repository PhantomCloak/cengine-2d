#pragma once
#include "editor_window.h"
#include "json.hpp"

class TilePlacer : public EditorWindow {
  public:
    void RenderWindow() override;
    bool IsFocused() override;
};
