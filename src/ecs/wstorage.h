#ifndef WSTORAGE_H
#define WSTORAGE_H

#include "../log/log.h"
#include "../eventmgr/EventBus.h"
#include "constants.h"
#include "pool.h"
#include <bitset>
#include <typeindex>
#include <unordered_map>

class System;

class WorldStorage {
    public:
    WorldStorage() {
        Log::Inf("WorldStroage is initialized");
    }
    ~WorldStorage();
    int entityNumberCtx = 0;
    std::vector<std::shared_ptr<IPool>> componentPools;
    std::vector<std::bitset<MAX_COMPONENT_COUNT>> entityComponentRegistry;
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
};

#endif
