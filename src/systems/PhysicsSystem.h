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
        RequireComponent<RectTransformC>();
        RequireComponent<RigidBody>();
    };

    void Update() override;
};
