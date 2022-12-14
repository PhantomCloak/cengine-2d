#pragma once
#include "../game/commanche2d.h"

class RenderText2D : public System {
    private:
    CommancheRenderer* renderer;

    public:
    RenderText2D(CommancheRenderer *renderer, SDL_Rect* camera) {
        this->renderer = renderer;
        RequireComponent<Label>();
    }

    void Update() {
        for (auto entities : GetEntities()) {
            const auto textLabel = entities.GetComponent<Label>();
            renderer->DrawText(AssetManager::GetFont(textLabel.fontName), textLabel.text, textLabel.pos.x, textLabel.pos.y, 300);
        }
    }
};
