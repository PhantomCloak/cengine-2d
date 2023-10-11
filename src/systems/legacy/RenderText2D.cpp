#include "RenderText2D.h"

void RenderText2D::Update() {
    if(!isActive)
      return;

    for (auto entities : GetEntities()) {
        const auto textLabel = entities.GetComponent<Label>();
        renderer->CDrawText(0, textLabel.text, textLabel.pos.x, textLabel.pos.y, 10, textLabel.textColor);
    }
}
