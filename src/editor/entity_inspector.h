#pragma once
#include "editor_window.h"
#include "flecs.h"

class Entity;
class EntityInspector : public EditorWindow {
  public:
    static void SetEntity(flecs::entity entity);
    bool IsFocused() override;
    void RenderWindow() override;
  private:
    static flecs::entity e;
};
