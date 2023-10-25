#include "systems.h"
#include "../assetmgr/AssetManager.h"
#include "../editor/editor.h"
#include "../io/cursor.h"
#include "../io/keyboard.h"
#include "../render/render.h"
#include "../scene/scene.h"
#include <numeric>
// flecs::query<RectTransformC, Sprite> sortedQuery;

void Systems::Init(flecs::world& ref) {
    ref.component<RectTransformC>();
    ref.component<Sprite>();
    ref.component<Label>();

    flecs::entity_t position_id = Scene::ecs.entity<Sprite>().id();

    ref.system<RectTransformC&, Sprite&>("RenderSystem")
    .iter(&Systems::RenderSystem);
    ref.system<RectTransformC&, Label&>("RenderTextSystem")
    .iter(&Systems::RenderTextSystem);
}


void Systems::RenderSystem(flecs::iter& it, RectTransformC* transform, Sprite* sprite) {
    static std::vector<std::pair<int, int>> sortedIndices;

    sortedIndices.clear();

    for (auto i = 0; i < it.count(); i++) {
        if (!it.entity(i).is_alive()) {
            continue;
        }
        sortedIndices.emplace_back(i, sprite[i].zIndex);
    }

    std::sort(sortedIndices.begin(), sortedIndices.end(),
    [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second < b.second;
    });

    for (const auto& [i, _] : sortedIndices) {
        if (!it.entity(i).is_alive()) {
            continue;
        }

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

    static bool drawGrid = false;
    if (Keyboard::IsKeyPressed(Key_C)) {
        Log::Inf("C key pressed");
        drawGrid = !drawGrid;
    }

    if (!drawGrid)
        CommancheRenderer::Instance->DrawGrids();
}


void Systems::RenderTextSystem(flecs::iter& it, RectTransformC* transform, Label* label) {
    for (auto i = 0; i < it.count(); i++) {
        if (!it.entity(i).is_alive())
            continue;
        CommancheRenderer::Instance->CDrawText(0, label[i].text, transform[i].pos.x, transform[i].pos.y, label[i].size, label[i].textColor);
    }
}
