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
    CommancheCamera* camera;
    CommancheRenderer* renderer;
    RenderDebug(CommancheRenderer* renderer, CommancheCamera* camera) {
        this->camera = camera;
        this->renderer = renderer;
        RequireComponent<BoxCollider>();
        RequireComponent<Transform>();
        RequireComponent<Sprite>();
    };

    void Update() {
        int gridSize = camera->gridSize;
        const int gridCtxW = 1920 / gridSize;
        const int gridCtxH = 1080 / gridSize;

        for (int i = 0; i < gridCtxW * gridCtxH; i++) {
            int nextX = (i * gridSize) % 1920;
            int nextY = floor(i / gridCtxW) * gridSize;

            //renderer->DrawRectangle(nextX, nextY, gridSize, gridSize,0 , { 255, 255, 255 });
        }

        for (auto entity : GetEntities()) {
            return;
            const auto& transform = entity.GetComponent<Transform>();

            CommancheRect colliderRect;
            colliderRect.x = transform.pos.x + camera->x;
            colliderRect.y = transform.pos.y + camera->y;
            colliderRect.width = camera->gridSize;
            colliderRect.height = camera->gridSize;

            //renderer->DrawRectangle(colliderRect.x, colliderRect.y, colliderRect.width, colliderRect.height, { 1, 0, 0 });
        }
    }
};

#endif
