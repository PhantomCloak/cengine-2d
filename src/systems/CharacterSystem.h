#pragma once
#include "../game/commanche2d.h"
#include "../io/keyboard.h"
#include "../physics/physics.h"

class CharacterSystem : public System {
    private:
    EventBus* eventBus;
    CommancheCamera* camera;

    public:
    CharacterSystem(EventBus* eventBus, CommancheCamera* camera) {
        this->eventBus = eventBus;
        this->camera = camera;

        RequireComponent<Sprite>();
        RequireComponent<RigidBody>();
        RequireComponent<RectTransform>();
        RequireComponent<CharacterController>();
    }

    void Update() override;
};
