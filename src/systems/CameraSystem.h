#pragma once
#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../ecs/system.h"
#include "../ecs/world.h"
#include "../game/map.h"
#include "../render/render_primitives.h"

class CameraSystem : public System {
    private:
    CommancheRect* camera;
    int mapWidth;
    int mapHeight;

    public:
    CameraSystem(CommancheRect* camera, Map* map) {
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

            camera->x = glm::clamp(transform.pos.x - camera->width / 2, 0.f, (float)mapWidth - camera->width);
            camera->y = glm::clamp(transform.pos.y - camera->height / 2, 0.f, (float)mapHeight - camera->height);
        }
    }
};
