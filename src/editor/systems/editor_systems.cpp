#include "editor_systems.h"
#include "../../components/editor_highlight.h"
#include "../../core/coordinate_system.h"
#include "../../io/cursor.h"
#include "../../scene/scene.h"
#include "../editor_utils.h"
#include <set>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
Editor* EditorSystems::editorRef;
flecs::entity_t lastSelectedEntity = 0;

bool entitySelected = false; // Flag to indicate if an entity was already selected


int compareZIndex(flecs::entity_t e1, const Sprite* s1, flecs::entity_t e2, const Sprite* s2) {
    return s2->zIndex - s1->zIndex;
}

void EditorSystems::Init(flecs::world& world, Editor* editor) {
    world.component<RectTransformC>();
    world.component<DragableComponent>();

    world.system<RectTransformC, DragableComponent>().each([&world](flecs::entity entity, RectTransformC& transform, const DragableComponent& comp) {
        DraggableSystem(world, entity, transform, comp);
    });


    world.system<RectTransformC, Sprite>().order_by(compareZIndex).each([](flecs::entity entity, RectTransformC& transform, Sprite& sprite) {
        ClickInspectSystem(Scene::ecs, entity, transform, sprite);
    });

    world.system<RectTransformC, Sprite, EditorHighlight>().each([](flecs::entity entity, RectTransformC& transform, Sprite& sprite, EditorHighlight& highlight) {
        if (highlight.Selected)
            sprite.color.a = 200;
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

bool isPointInsideRotatedRect(const glm::vec2& point, const glm::vec2& center, const glm::vec2& size, float rotation) {
    rotation = rotation * M_PI / 180.0f;
    glm::vec2 translatedPoint = point - center;

    float s = sin(-rotation);
    float c = cos(-rotation);
    glm::vec2 rotatedPoint = glm::vec2(translatedPoint.x * c - translatedPoint.y * s, translatedPoint.x * s + translatedPoint.y * c);

    return abs(rotatedPoint.x) <= size.x && abs(rotatedPoint.y) <= size.y;
}


void EditorSystems::ClickInspectSystem(flecs::world& world, flecs::entity entity, RectTransformC& transform, Sprite& sprite) {

    if (!Cursor::HasLeftCursorClicked()) {
        entitySelected = false;
        return;
    }

    if(!editorRef->viewport->IsFocused())
        return;

    return;

    auto worldCursorPos = Cursor::GetCursorWorldPosition(Editor::GetCursorPosition(), CommancheRenderer::Instance->camX);


    if (isPointInsideRotatedRect(worldCursorPos, transform.pos, transform.size, transform.rotation)) {
        if (!entitySelected) {
            printf("entity selected id %llu\n", entity.id());
            entity.set<EditorHighlight>({ true });

            if (lastSelectedEntity != 0 && lastSelectedEntity != entity.id()) {
                printf("last selected entity id %llu\n", lastSelectedEntity);
                auto lastSelectedEntityRef = Scene::ecs.entity(lastSelectedEntity);
                auto sp = lastSelectedEntityRef.get_ref<Sprite>();
                auto hl = lastSelectedEntityRef.get_ref<EditorHighlight>();
                hl->Selected = false;
                sp->color.a = 255;
                lastSelectedEntityRef.remove<EditorHighlight>();
                editorRef->sceneList->SelectEntity(lastSelectedEntity);
            }
            lastSelectedEntity = entity;


            entitySelected = true;
            return;
        }
    }
};

void EditorSystems::Update() {
}
