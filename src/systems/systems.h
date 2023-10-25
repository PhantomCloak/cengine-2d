#include "../components/RectTransform.h"
#include "../components/DebugTile.h"
#include "../components/Sprite.h"
#include "../components/Label.h"
#include "flecs.h"

class Systems {
public:
    static void Init(flecs::world& world);
    static void RenderSystem(flecs::iter& it, RectTransformC* transform, Sprite* sprite);
    static void RenderTextSystem(flecs::iter& it, RectTransformC* transform, Label* label);
};
