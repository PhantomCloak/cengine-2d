#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ecs/entity.h"
#include "../eventmgr/EventBus.h"

class CollisionEvent {
    public:
    Entity a;
    Entity b;
    CollisionEvent(Entity a, Entity b)
    : a(a), b(b){};
};
#endif
