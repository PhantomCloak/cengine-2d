#pragma once
#include "../game/commanche2d.h"

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
        RequireComponent<CharacterController>();
    }

    void SubscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
        // eventBus->SubscribeEvent(this, &CharacterSystem::onKeyPressed);
    }

    void onKeyKeyPressed(KeyPressUpEvent& event) {
        //if (event.key == SDLK_1) {
        //    camera->gridSize *= 2;
        //}

        //if (event.key == SDLK_2) {
        //    camera->gridSize /= 2;
        //}

        //for (auto entity : GetEntities()) {
        //    auto& rigidbody = entity.GetComponent<RigidBody>();
        //    const auto characterController = entity.GetComponent<CharacterController>();
        //    if (event.key == SDLK_UP) {
        //      Log::Inf("JUMP HAS PRESSED");
        //        rigidbody.forceAcc += characterController.upVelocity;
        //    }
        //}
    }

    void onKeyPress(KeyPressEvent& event) {
        //for (auto entity : GetEntities()) {
        //    const auto characterController = entity.GetComponent<CharacterController>();
        //    auto& rigidbody = entity.GetComponent<RigidBody>();

        //    if (event.keyboard[SDL_SCANCODE_RIGHT]) {
        //      Log::Inf("RIGHT HAS PRESSED");
        //        rigidbody.forceAcc += characterController.rightVelocity;
        //    }

        //    if (event.keyboard[SDL_SCANCODE_LEFT]) {
        //      Log::Inf("LEFT HAS PRESSED");
        //        rigidbody.forceAcc += characterController.leftVelocity;
        //    }
        //}
    }

    void Update() {
        auto keyHoldEvents = eventBus->GetEvents<KeyPressEvent>();
        auto keyPressedEvents = eventBus->GetEvents<KeyPressUpEvent>();
        for (auto event : keyHoldEvents) {
            onKeyPress(event);
        }
        for (auto event : keyPressedEvents) {
            onKeyKeyPressed(event);
        }
    }
};
