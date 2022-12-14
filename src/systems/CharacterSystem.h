#pragma once
#include "../game/commanche2d.h"

class CharacterSystem : public System {
    private:
    std::shared_ptr<EventBus> eventBus;

    public:
    CharacterSystem (std::shared_ptr<EventBus> eventBus) {
        this->eventBus = eventBus;

        RequireComponent<Sprite>();
        RequireComponent<RigidBody>();
        RequireComponent<CharacterController>();

        SubscribeToEvents(eventBus);
    }

    void SubscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
        eventBus->SubscribeEvent(this, &CharacterSystem::onKeyPressed);
    }

    void onKeyPressed(KeyPressedEvent& event) {
        for (auto entity : GetEntities()) {
            const auto keyboardcontrol = entity.GetComponent<CharacterController>();
            auto& sprite = entity.GetComponent<Sprite>();
            auto& rigidbody = entity.GetComponent<RigidBody>();

            switch (event.symbol) {
            case SDLK_UP:
                rigidbody.velocity = keyboardcontrol.upVelocity;
                sprite.srcRect.y = sprite.height * 0;
                break;
            case SDLK_RIGHT:
                rigidbody.velocity = keyboardcontrol.rightVelocity;
                sprite.srcRect.y = sprite.height * 1;
                break;
            case SDLK_DOWN:
                rigidbody.velocity = keyboardcontrol.downVelocity;
                sprite.srcRect.y = sprite.height * 2;
                break;
            case SDLK_LEFT:
                rigidbody.velocity = keyboardcontrol.leftVelocity;
                sprite.srcRect.y = sprite.height * 3;
                break;
            }
        }
    }
};
