#include "systems.h"
#include "../assetmgr/AssetManager.h"
#include "../editor/editor.h"
#include "../io/cursor.h"
#include "../render/render.h"
#include <numeric>
//flecs::query<RectTransformC, Sprite> sortedQuery;

void Systems::Init(flecs::world& ref) {
    ref.component<RectTransformC>();
    ref.component<Sprite>();
    ref.system<RectTransformC&, Sprite&>("RenderSystem")
    .iter(&Systems::RenderSystem);
}


void Systems::RenderSystem(flecs::iter& it, RectTransformC* transform, Sprite* sprite) {

    for (auto i = 0; i < it.count(); i++) {
        if (!it.entity(i).is_alive()) {
            continue;
        }

        // Ensure texture is loaded
        if (sprite[i].texture == 0) {
            sprite[i].texture = AssetManager::GetTexture(sprite[i].textureId);
        }

        CommancheRenderer::Instance->CDrawImage(sprite[i].texture,
        transform[i].pos.x,
        transform[i].pos.y,
        transform[i].size.x,
        transform[i].size.y,
        transform[i].rotation,
        sprite[i].srcRect.x,
        sprite[i].srcRect.y,
        sprite[i].srcRect.width,
        sprite[i].srcRect.height,
        sprite[i].color);
    }

    CommancheRenderer::Instance->DrawGrids();
}
