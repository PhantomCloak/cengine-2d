#ifndef COLLISIONRESOLVER_H
#define COLLISIONRESOLVER_H

#include "../components/BoxCollider.h"
#include "../components/TileCollider.h"
#include "../components/Transform.h"
#include "../ecs/system.h"
#include "../ecs/world.h"
#include "../events/CollisionEvent.h"

class CollisionResolver : public System {
    private:
    std::shared_ptr<EventBus> eventBus;

    public:
    CollisionResolver(std::shared_ptr<EventBus> eventBus) {
        this->eventBus = eventBus;
        RequireComponent<Transform>();
        RequireComponent<BoxCollider>();
    };

    void Update() {
        for (Entity& a : GetEntities()) {

            auto aCol = a.GetComponent<BoxCollider>();
            auto aTransf = a.GetComponent<Transform>();

            aCol.width *= aTransf.scale.x;
            aCol.height *= aTransf.scale.y;

            for (Entity& b : GetEntities()) {
                if (a == b)
                    continue;

                auto bCol = b.GetComponent<BoxCollider>();
                auto bTransf = b.GetComponent<Transform>();

                aTransf.pos.x += bCol.offset.x;
                aTransf.pos.y += bCol.offset.y;

                bTransf.pos.x += bCol.offset.x;
                bTransf.pos.y += bCol.offset.y;

                bCol.width *= bTransf.scale.x;
                bCol.height *= bTransf.scale.y;

                bool isColliding =
                aTransf.pos.x < bTransf.pos.x + bCol.width &&
                aTransf.pos.x + aCol.width > bTransf.pos.x &&
                aTransf.pos.y < bTransf.pos.y + bCol.height &&
                aTransf.pos.y + aCol.height > bTransf.pos.y;

                if (isColliding) {
                    Log::Err("Collided!");
                    //eventBus->EmitEvent<CollisionEvent>(a, b);
                }
            }
        }
    }
};
#endif
