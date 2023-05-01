#pragma once
#include "editor_window.h"

class Entity;
class EntityInspector : EditorWindow {
  public:
    static void SetEntity(Entity entity);
    void RenderWindow() override;
  private:
    static Entity e;
};
