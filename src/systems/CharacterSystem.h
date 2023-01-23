#pragma once
#include "../game/commanche2d.h"
#include "../core/keyboard.h"

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

    void Update() {
        for (auto entity : GetEntities()) {
            const auto characterController = entity.GetComponent<CharacterController>();
            auto& rigidbody = entity.GetComponent<RigidBody>();
            auto& transform = entity.GetComponent<RectTransform>();

            if (Keyboard::IsKeyPressed(KeyCode::Key_W)) {
                Log::Inf("UP HAS PRESSED");
                rigidbody.forceAcc += characterController.upVelocity;
            }
            if (Keyboard::IsKeyPressing(KeyCode::Key_D)) {
                Log::Inf("RIGHT HAS PRESSED");
                rigidbody.forceAcc += characterController.rightVelocity;
                //transform.pos.x += 1;
            }

            if (Keyboard::IsKeyPressing(KeyCode::Key_A)) {
                Log::Inf("LEFT HAS PRESSED");
                //transform.pos.x -= 1;
                rigidbody.forceAcc += characterController.leftVelocity;
            }
        }
    }
};
