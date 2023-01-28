#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../ecs/system.h"
#include "../components/RigidBody.h"
#include "../components/Transform.h"
#include "../log/log.h"

class MovementSystem : public System
{
  public:
    MovementSystem()
    {
      RequireComponent<Transform>();
      RequireComponent<RigidBody>();
    };

    void Update(float dt)
    {
      for(auto entity : GetEntities()){
        return;
        auto& transform = entity.GetComponent<Transform>();
        const auto rigidbody = entity.GetComponent<RigidBody>();

        transform.pos.x += rigidbody.velocityLinear.x * dt;
        transform.pos.y += rigidbody.velocityLinear.y * dt;
      }
    };
};

#endif
