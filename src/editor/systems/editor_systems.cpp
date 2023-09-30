#include "editor_systems.h"
#include "../../io/cursor.h"
#include "../editor_utils.h"

Editor* EditorSystems::editorRef;

void EditorSystems::Init(flecs::world& world, Editor* editor) {
    world.component<RectTransform>();
    world.component<DragableComponent>();

    world.system<RectTransform, DragableComponent>().each([&world](flecs::entity entity, RectTransform& transform, const DragableComponent& comp) {
        DraggableSystem(world, entity, transform, comp);
    });

    editorRef = editor;
}

void EditorSystems::DraggableSystem(flecs::world& world, flecs::entity entity, RectTransform& transform, const DragableComponent& comp) {
    auto mPos = Cursor::GetCursorPosition();
    glm::vec2 local_mouse_pos(mPos.x - editorRef->ViewportPos.x, mPos.y - editorRef->ViewportPos.y);
    glm::vec2 worldPos = EditorUtils::InterpolateToGrid(Cursor::GetCursorWorldPosition(local_mouse_pos), 25);
    transform.pos = worldPos;

    if (Cursor::HasLeftCursorClicked()) {
        world.entity(entity).remove<DragableComponent>();
    } else if (Cursor::HasRightCursorClicked()) {
        entity.destruct();
    }
}
