#include "editor_systems.h"
#include "../../io/cursor.h"
#include "../editor_utils.h"

Editor* EditorSystems::editorRef;
flecs::world* ecs;

void EditorSystems::Init(flecs::world* world, Editor* editor) {
    world->component<RectTransform>();
    world->component<DragableComponent>();
    //world->system<RectTransform, DragableComponent>("Editor_DraggableSystem")
    //.each(EditorSystems::DraggableSystem);


     world->system<RectTransform, DragableComponent>().each([](flecs::entity entity, RectTransform& transform, const DragableComponent& comp) {
         EditorSystems::DraggableSystem(entity, transform);
     });

    editorRef = editor;
    ecs = world;
}

void EditorSystems::DraggableSystem(flecs::entity entity, RectTransform& transform) {
    auto mPos = Cursor::GetCursorPosition();
    glm::vec2 local_mouse_pos(mPos.x - editorRef->ViewportPos.x, mPos.y - editorRef->ViewportPos.y);
    glm::vec2 worldPos = EditorUtils::InterpolateToGrid(Cursor::GetCursorWorldPosition(local_mouse_pos), 25);
    transform.pos = worldPos;

    if (Cursor::HasCursorClicked()) {
        ecs->entity(entity).remove<DragableComponent>();
    }
}
