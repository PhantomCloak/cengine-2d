#pragma once
#include "../ecs/world.h"
#include "commanche2d.h"

class Wrapper {
    public:
    Wrapper(std::shared_ptr<World> world);
    void AddCollider(int entityId, int size);
    private:
    std::shared_ptr<World> world;
};
