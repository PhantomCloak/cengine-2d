#include "CharacterSystem.h"

void CharacterSystem::Update() {
    if (!isActive)
        return;

    for (auto entity : GetEntities()) {
        auto& characterController = entity.GetComponent<CharacterController>();
        auto& rigidbody = entity.GetComponent<RigidBody>();
        auto& transform = entity.GetComponent<RectTransformC>();

        if (Keyboard::IsKeyPressed(KeyCode::Key_W)) {
            Physics::AddForce(rigidbody, characterController.upVelocity);
            characterController.up = true;
        } else {
            characterController.up = false;
        }
        if (Keyboard::IsKeyPressing(KeyCode::Key_D)) {
            transform.pos.x += characterController.rightVelocity.x;
            characterController.right = true;
        } else {
            characterController.right = false;
        }

        if (Keyboard::IsKeyPressing(KeyCode::Key_A)) {
            transform.pos.x -= characterController.rightVelocity.x;
            characterController.left = true;
        } else
            characterController.left = false;
    }
}
