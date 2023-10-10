#pragma once
#include "editor_window.h"
#include "flecs.h"

class SceneList : EditorWindow {
  public:
    void SetSelectCallback(std::function<void(flecs::entity)> callback = nullptr);
    void RenderWindow() override;
};


