#include "editor_systems.h"
#include "../../core/coordinate_system.h"
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
    auto mPos = Cursor::GetCursorWorldPosition(glm::vec2(Editor::Instance->x, Editor::Instance->y), CommancheRenderer::Instance->camX);
    glm::vec2 pos = glm::vec2(mPos.x, mPos.y);

    transform.pos = EditorUtils::InterpolateToGrid(pos, 2.5);

    if (Cursor::HasLeftCursorClicked()) {
        world.entity(entity).remove<DragableComponent>();
    } else if (Cursor::HasRightCursorClicked()) {
        entity.destruct();
    }
}
