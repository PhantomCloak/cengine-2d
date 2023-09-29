#pragma once
#include "editor_window.h"
#include "flecs.h"

class Entity;
class EntityInspector : EditorWindow {
  public:
    static void SetEntity(flecs::entity entity);
    void RenderWindow() override;
  private:
    static flecs::entity e;
};
