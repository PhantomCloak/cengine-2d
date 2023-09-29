#include "flecs.h"
#include "../editor.h"
#include "../../components/RectTransform.h"
#include "../../components/DragableComponent.h"
#include "../../components/Sprite.h"

class EditorSystems {
public:
    static void Init(flecs::world* world, Editor* editor);
    static void DraggableSystem(flecs::entity it, RectTransform& transform);
    static Editor* editorRef;
};
