#include "systems.h"
#include "../render/render.h"

struct RenderObject {
    std::shared_ptr<RectTransform> transform;
    std::shared_ptr<Sprite> sprite;
};

void Systems::Init(flecs::world& ref){
       ref.component<RectTransform>();
       ref.component<Sprite>();
       ref.system<RectTransform, Sprite>("RenderSystem")
               .iter(&Systems::RenderSystem);
}

void Systems::RenderSystem(flecs::iter& it, RectTransform* transform, Sprite* sprite) {
    std::vector<RenderObject> renderSortList;

    for (auto i = 0; i < it.count(); i++) {
        if (it.entity(i).has<DebugTile>()) {
            continue;
        }

        RenderObject renderObj = {
            .sprite = std::make_unique<Sprite>(sprite[i]),
            .transform = std::make_unique<RectTransform>(transform[i])
        };

        renderSortList.push_back(renderObj);
    }

    std::sort(renderSortList.begin(), renderSortList.end(), [](const RenderObject& a, const RenderObject& b) {
        return a.sprite->zIndex < b.sprite->zIndex;
    });

    for (RenderObject& renderObj : renderSortList) {
            CommancheRenderer::Instance->CDrawImage(renderObj.sprite->texture,
                            renderObj.transform->pos.x,
                            renderObj.transform->pos.y,
                            renderObj.transform->size.x,
                            renderObj.transform->size.y,
                            renderObj.transform->rotation,
                            renderObj.sprite->srcRect.x,
                            renderObj.sprite->srcRect.y,
                            renderObj.sprite->srcRect.width,
                            renderObj.sprite->srcRect.height);
    }

    CommancheRenderer::Instance->DrawGrids();
}


