#pragma once

#include "../ecs/world.h"
#include "../eventmgr/EventBus.h"
#include "../render/render.h"
#include "../components/Camera.h"
#include "../game/map.h"
#include "../events/KeyPressedEvent.h"
#include "fileview.h"
#include "system_explorer.h"
#include "entity_inspector.h"

class Editor {
    public:
    Editor() = default;
    void Init(CommancheRenderer* renderer);
    void Render();
    static glm::vec2 ScreenSize;
    SystemExplorer* explorer;
    glm::vec2 ViewportPos;
    glm::vec2 ViewportSize;
    private:
    void Keybindings();
    const int gridSize = 25;
    FileView* fileView;
    EntityInspector* entityInspector;
    CommancheRenderer* renderer;
};
