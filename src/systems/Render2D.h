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
    CommancheCamera* camera;
    CommancheRenderer* renderer;

    void RenderParticles();
    void RenderSprites();

    public:
    RenderSystem(CommancheRenderer* renderer, CommancheCamera* camera) {
        this->camera = camera;
        this->renderer = renderer;
        // RequireComponent<Transform>();
        RequireComponent<RectTransform>();
        RequireComponent<Sprite>();
    };

    void Update() {
        std::vector<RenderObject> renderSortList;

        for (auto entity : GetEntities()) {
            auto& transform = entity.GetComponent<RectTransform>();
            auto sprite = entity.GetComponent<Sprite>();
            bool term = entity.HasComponent<DebugTile>();

            if(term)
            {
              continue;
            }

            RenderObject renderObj = {
                .sprite = std::make_unique<Sprite>(sprite),
                .transform = std::make_unique<RectTransform>(transform)
            };

            renderSortList.push_back(renderObj);
        }

        std::sort(renderSortList.begin(), renderSortList.end(), [](const RenderObject& a, const RenderObject& b) {
            return a.sprite->zIndex < b.sprite->zIndex;
        });

        for (RenderObject& renderObj : renderSortList) {
            //if (renderObj.transform->size.x < 1.0f) {
                //renderer->DrawRectRangle(renderObj.sprite->texture, renderObj.transform->pos.x, renderObj.transform->pos.y, renderObj.transform->size.x, renderObj.transform->size.y, renderObj.transform->rotation, 0, 0);
            //} else
                renderer->CDrawImage(renderObj.sprite->texture, renderObj.transform->pos.x, renderObj.transform->pos.y, renderObj.transform->size.x, renderObj.transform->size.y, renderObj.transform->rotation, 0, 0);
        }

        // renderer->CDrawText(0, "C ENGINE 2D", 600, 100, 10, {255, 0 , 0});
        renderer->CDrawLine(0, 0, 10, 500);
        renderer->DrawTest();
    };
};
