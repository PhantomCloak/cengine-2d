#include "PhysicsSystem.h"

void PhysicsController::Update() {
    if (!isActive)
        return;

    for (auto entity : GetEntities()) {
        RectTransformC transform = entity.GetComponent<RectTransformC>();
        RigidBody& rigidBody = entity.GetComponent<RigidBody>();
        rigidBody.rigId = entity.GetId();

        if (!Physics::HasRegistered(entity.GetId())) {
            Physics::RegisterBody(rigidBody, transform);
        }

        Physics::SetPosition(rigidBody, transform.pos);
        Physics::SetVelocity(rigidBody, rigidBody.velocityLinear);
    }

    Physics::Update();

    std::map<int, PhysicsResult> updatedPhysicsObjs = Physics::GetUpdatedEntries();
    for (auto entity : GetEntities()) {
        RectTransformC& transform = entity.GetComponent<RectTransformC>();
        RigidBody& rigidBody = entity.GetComponent<RigidBody>();

        PhysicsResult& result = updatedPhysicsObjs[entity.GetId()];

        transform.pos = result.position;
        transform.rotation = result.rotation;
        rigidBody.velocityLinear = result.velocityLinear;
        rigidBody.velocityAngular = result.velocityAngular;
        rigidBody.mass = result.mass;
    }
}
