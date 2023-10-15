#ifndef RENDERDBG_H
#define RENDERDBG_H

#include "../components/BoxCollider.h"
#include "../components/Sprite.h"
#include "../components/RectTransform.h"
#include "../components/DebugTile.h"
#include "../ecs/system.h"
#include "../log/log.h"
#include "../src/render/render.h"

class RenderDebug : public System {
    public:
    CommancheRenderer* renderer;
    RenderDebug(CommancheRenderer* renderer) {
        this->renderer = renderer;
        RequireComponent<RectTransformC>();
        RequireComponent<Sprite>();
        RequireComponent<DebugTile>();
    };

    void Update() {
        for (auto entity : GetEntities()) {
          auto& transform = entity.GetComponent<RectTransformC>();
          auto sprite = entity.GetComponent<Sprite>();

          //renderer->DrawRectRangle(sprite.texture, transform.pos.x, transform.pos.y, transform.size.x, transform.size.y, 0, 0, 0);
        }
    }
};

#endif
