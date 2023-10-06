#include "editor_systems.h"
#include "../../io/cursor.h"
#include "../editor_utils.h"

Editor* EditorSystems::editorRef;

void EditorSystems::Init(flecs::world& world, Editor* editor) {
    world.component<RectTransformC>();
    world.component<DragableComponent>();

    world.system<RectTransformC, DragableComponent>().each([&world](flecs::entity entity, RectTransformC& transform, const DragableComponent& comp) {
        DraggableSystem(world, entity, transform, comp);
    });

    editorRef = editor;
}

void EditorSystems::DraggableSystem(flecs::world& world, flecs::entity entity, RectTransformC& transform, const DragableComponent& comp) {
    //auto mPos = GetScreenToWorld2D(Vector2{ Editor::Instance->x, Editor::Instance->y }, CommancheRenderer::Instance->camX);
    //transform.pos = glm::vec2(mPos.x, mPos.y);

    //if (Cursor::HasLeftCursorClicked()) {
    //    world.entity(entity).remove<DragableComponent>();
    //} else if (Cursor::HasRightCursorClicked()) {
    //    entity.destruct();
    //}
}
