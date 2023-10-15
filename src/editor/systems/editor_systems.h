#include "flecs.h"
#include "../editor.h"
#include "../../components/RectTransform.h"
#include "../../components/DragableComponent.h"
#include "../../components/Sprite.h"

class EditorSystems {
public:
    static void Init(flecs::world& world, Editor* editor);
    static void DraggableSystem(flecs::world& world, flecs::entity it, RectTransformC& transform, const DragableComponent& comp);
    static void ClickInspectSystem(flecs::world& world, flecs::entity it, RectTransformC& transform, Sprite& sprite);
    static void Update();
    static Editor* editorRef;
};
