#pragma once

#include "../components/RectTransform.h"
#include "../components/Sprite.h"
#include "../components/Transform.h"
#include "../components/DebugTile.h"
#include "../ecs/system.h"
#include "../log/log.h"
#include "../render/render.h"

struct RenderObject {
    std::shared_ptr<RectTransform> transform;
    std::shared_ptr<Sprite> sprite;
};

class RenderSystem : public System {
    private:
    CommancheRenderer* renderer;

    void RenderParticles();
    void RenderSprites();

    public:
    RenderSystem(CommancheRenderer* renderer) {
        this->renderer = renderer;
        RequireComponent<RectTransform>();
        RequireComponent<Sprite>();
    };

    void Update() override;
};
