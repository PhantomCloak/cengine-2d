#include "Render2D.h"

void RenderSystem::RenderParticles() {
}

void RenderSystem::RenderSprites() {
}

void RenderSystem::Update() {
    if (!isActive)
        return;

    std::vector<RenderObject> renderSortList;

    for (auto entity : GetEntities()) {
        auto& transform = entity.GetComponent<RectTransformC>();
        auto sprite = entity.GetComponent<Sprite>();
        bool term = entity.HasComponent<DebugTile>();

        if (term) {
            continue;
        }

        RenderObject renderObj = {
            .sprite = std::make_unique<Sprite>(sprite),
            .transform = std::make_unique<RectTransformC>(transform)
        };

        renderSortList.push_back(renderObj);
    }

    std::sort(renderSortList.begin(), renderSortList.end(), [](const RenderObject& a, const RenderObject& b) {
        return a.sprite->zIndex < b.sprite->zIndex;
    });

    //for (RenderObject& renderObj : renderSortList) {
    //        renderer->CDrawImage(renderObj.sprite->texture,
    //                        renderObj.transform->pos.x,
    //                        renderObj.transform->pos.y,
    //                        renderObj.transform->size.x,
    //                        renderObj.transform->size.y,
    //                        renderObj.transform->rotation,
    //                        renderObj.sprite->srcRect.x,
    //                        renderObj.sprite->srcRect.y,
    //                        renderObj.sprite->srcRect.width,
    //                        renderObj.sprite->srcRect.height);
    //}
    //renderer->DrawGrids();
};
