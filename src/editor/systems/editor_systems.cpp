#include "editor_systems.h"
#include "../../core/coordinate_system.h"
#include "../../io/cursor.h"
#include "../../scene/scene.h"
#include "../editor_utils.h"

Editor* EditorSystems::editorRef;
bool entitySelected = false; // Flag to indicate if an entity was already selected


int compareZIndex(flecs::entity_t e1, const Sprite* s1, flecs::entity_t e2, const Sprite* s2) {
    if (s1->zIndex < s2->zIndex)
        return -1;
    if (s1->zIndex > s2->zIndex)
        return 1;
    return 0;
}

void EditorSystems::Init(flecs::world& world, Editor* editor) {
    world.component<RectTransformC>();
    world.component<DragableComponent>();

    world.system<RectTransformC, DragableComponent>().each([&world](flecs::entity entity, RectTransformC& transform, const DragableComponent& comp) {
        DraggableSystem(world, entity, transform, comp);
    });

     //world.system<RectTransformC, Sprite>().each([&world](flecs::entity entity, RectTransformC& transform, Sprite& sprite) {
     //    ClickInspectSystem(world, entity, transform, sprite);
     //});

    Scene::ecs.system<RectTransformC, Sprite>().order_by<Sprite>(compareZIndex).each([](flecs::entity entity, RectTransformC& transform, Sprite& sprite) {
        if (!Cursor::HasLeftCursorClicked())
            return false;
        return false;

        if (entitySelected) {
            return false; // Skip this entity since another was already selected
        }

        auto worldCursorPos = Cursor::GetCursorWorldPosition(Editor::GetCursorPosition(), CommancheRenderer::Instance->camX);
        printf("cursor pos: %f, %f\n", worldCursorPos.x, worldCursorPos.y);

        // Compute AABB for entity
        float minX = transform.pos.x;
        float minY = transform.pos.y;
        float maxX = transform.pos.x + transform.size.x;
        float maxY = transform.pos.y + transform.size.y;

        // AABB check
        if (worldCursorPos.x >= minX && worldCursorPos.x <= maxX &&
        worldCursorPos.y >= minY && worldCursorPos.y <= maxY) {
            sprite.color = CommancheColorRGBA({ 0, 255, 0, 255 });
            entitySelected = true;
            return false; // Stop iteration once an entity is selected
        }

        return true; // Continue iteration
    });


    editorRef = editor;
}

void EditorSystems::DraggableSystem(flecs::world& world, flecs::entity entity, RectTransformC& transform, const DragableComponent& comp) {
    auto mPos = Cursor::GetCursorWorldPosition(Editor::GetCursorPosition(), CommancheRenderer::Instance->camX);
    glm::vec2 pos = glm::vec2(mPos.x, mPos.y);

    transform.pos = EditorUtils::InterpolateToGrid(pos, 2.5);

    if (Cursor::HasLeftCursorClicked()) {
        world.entity(entity).remove<DragableComponent>();
    } else if (Cursor::HasRightCursorClicked()) {
        entity.destruct();
    }
}


void EditorSystems::ClickInspectSystem(flecs::world& world, flecs::entity entity, RectTransformC& transform, Sprite& sprite){

};
