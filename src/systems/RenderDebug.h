#ifndef RENDERDBG_H
#define RENDERDBG_H

#include "../components/BoxCollider.h"
#include "../components/Camera.h"
#include "../components/Sprite.h"
#include "../components/Transform.h"
#include "../ecs/system.h"
#include "../log/log.h"
#include "../src/render/render.h"

class RenderDebug : public System {
    public:
    SDL_Rect* camera;
    CommancheRenderer* renderer;
    RenderDebug(CommancheRenderer* renderer, SDL_Rect* camera) {
        this->camera = camera;
        this->renderer = renderer;
        RequireComponent<BoxCollider>();
        RequireComponent<Transform>();
        RequireComponent<Sprite>();
    };

    void Update() {
        for (auto entity : GetEntities()) {
            const auto& collider = entity.GetComponent<BoxCollider>();
            const auto& transform = entity.GetComponent<Transform>();

            SDL_Rect colliderRect = {
                .x = (int)(transform.pos.x + collider.offset.x - camera->x),
                .y = (int)(transform.pos.y + collider.offset.y - camera->y),
                .w = (int)(collider.width * transform.scale.x),
                .h = (int)(collider.height * transform.scale.y),
            };

            renderer->DrawRectangle(colliderRect.x, colliderRect.y, colliderRect.w, colliderRect.h, {1, 0, 0});
        }
    }
};

#endif
