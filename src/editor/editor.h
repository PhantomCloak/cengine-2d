#pragma once

#include "../ecs/world.h"
//#include <SDL.h>
#include "../eventmgr/EventBus.h"
#include "../render/render.h"
#include "../components/Camera.h"
#include "../game/map.h"
#include "../events/KeyPressedEvent.h"
#include "fileview.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
//#include <SDL_opengles2.h>
#else
//#include <SDL_opengl.h>
#endif

class Editor {
    public:
    Editor() = default;
    void Init(CommancheRenderer* renderer, Map* map,std::shared_ptr<World> world, CommancheCamera * camera, EventBus* eventBus);
    void Render();
    private:
    void Keybindings();
    FileView* fileView;
    void onMousePressed(MousePressedEvent& event);
    std::shared_ptr<World> world;
    CommancheRenderer* renderer;
};
