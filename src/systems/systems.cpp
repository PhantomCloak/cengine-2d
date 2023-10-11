#include "systems.h"
#include "../editor/editor.h"
#include "../render/render.h"
#include "../io/cursor.h"
#include "../assetmgr/AssetManager.h"
#include <numeric>

flecs::query<RectTransformC, Sprite> sortedQuery;

void Systems::Init(flecs::world& ref) {
    ref.component<RectTransformC>();
    ref.component<Sprite>();

    // Define a sorted query
    sortedQuery = ref.query_builder<RectTransformC, Sprite>()
        .order_by<Sprite>([](flecs::entity_t e1, const Sprite *s1, flecs::entity_t e2, const Sprite *s2) {
            return (s1->zIndex > s2->zIndex) - (s1->zIndex < s2->zIndex);
        })
        .build();

    ref.system<RectTransformC&, Sprite&>("RenderSystem")
        .iter(&Systems::RenderSystem);
}


void Systems::RenderSystem(flecs::iter& it, RectTransformC* transform, Sprite* sprite) {

    for (auto i = 0; i < it.count(); i++) {
        if (it.entity(i).has<DebugTile>()) {
            continue; // Skip entities with DebugTile component
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
