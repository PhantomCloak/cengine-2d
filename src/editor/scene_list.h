#pragma once
#include "editor_window.h"
#include "flecs.h"
#include <functional>

class SceneList : public EditorWindow {
  public:
    void SetSelectCallback(std::function<void(flecs::entity)> callback = nullptr);
    void SelectEntity(flecs::entity_t entity);
    bool IsFocused() override;
    void RenderWindow() override;
};


