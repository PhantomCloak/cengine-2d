#pragma once
#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../ecs/system.h"
#include "../ecs/world.h"
#include "../game/map.h"
#include "SDL_rect.h"

class CameraSystem : public System {
    private:
    SDL_Rect* camera;
    int mapWidth;
    int mapHeight;

    public:
    CameraSystem(SDL_Rect* camera, Map* map) {
        this->camera = camera;
        //this->mapWidth = map->GetMapWidth();
        //this->mapHeight = map->GetMapHeight();
        this->mapWidth = 1920;
        this->mapHeight = 1080;
        RequireComponent<Camera>();
    }

    void Update() {
        for (auto& entity : GetEntities()) {
            auto& transform = entity.GetComponent<Transform>();

            camera->x = glm::clamp(transform.pos.x - camera->w / 2, 0.f, (float)mapWidth - camera->w);
            camera->y = glm::clamp(transform.pos.y - camera->h / 2, 0.f, (float)mapHeight - camera->h);
        }
    }
};
