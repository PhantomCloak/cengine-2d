#pragma once
#include "../ecs/system.h"
#include "../game/components.h"
#include "../physics/physics.h"
#include "../render/physics_debug_renderer.h"
#include "box2d/box2d.h"

#define PIXELS_PER_METER 10

struct PhyscsBinding {
    Entity entity;
    b2Body* body;

    PhyscsBinding(Entity entity) {
        this->entity = entity;
        this->body = nullptr;
    }
};

class PhysicsController : public System {
    private:
    const b2Vec2 grav = b2Vec2(0.0f, 10.0f * PIXELS_PER_METER);
    b2World* world = new b2World(grav);
    std::vector<PhyscsBinding*> bindings;

    public:
    PhysicsController() {
        RequireComponent<Sprite>();
        RequireComponent<RectTransform>();
        RequireComponent<RigidBody>();
    };

    void Update() {

        for (auto entity : GetEntities()) {
            RectTransform transform = entity.GetComponent<RectTransform>();
            RigidBody& rigidBody = entity.GetComponent<RigidBody>();
            rigidBody.rigId = entity.GetId();

            if (!Physics::HasRegistered(entity.GetId())) {
                Physics::RegisterBody(rigidBody, transform);
            }
            Physics::SetPosition(rigidBody, transform.pos);
        }

        Physics::Update();

        std::map<int, PhysicsResult> updatedPhysicsObjs = Physics::GetUpdatedEntries();
        for (auto entity : GetEntities()) {
            RectTransform& transform = entity.GetComponent<RectTransform>();
            RigidBody& rigidBody = entity.GetComponent<RigidBody>();

            PhysicsResult& result = updatedPhysicsObjs[entity.GetId()];

            transform.pos = result.position;
            transform.rotation = result.rotation;
            rigidBody.velocityLinear = result.velocityLinear;
            rigidBody.velocityAngular = result.velocityAngular;
            rigidBody.mass = result.mass;
        }
    }
};
