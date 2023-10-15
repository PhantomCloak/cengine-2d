#pragma once
#include "../game/commanche2d.h"

class RenderText2D : public System {
    private:
    CommancheRenderer* renderer;

    public:
    RenderText2D(CommancheRenderer *renderer) {
        this->renderer = renderer;
        RequireComponent<Label>();
    }

    void Update() override;
};
