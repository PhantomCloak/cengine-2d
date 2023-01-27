#ifndef WORLD_H
#define WORLD_H

#include "../eventmgr/EventBus.h"
#include "../log/log.h"
#include "entity.h"
#include "pool.h"
#include "serializer.h"
#include "system.h"
#include "wstorage.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

class World {
    private:
    std::set<Entity> entityAddQueue;
    std::set<Entity> entityRemoveQueue;
    std::deque<int> freeIds;

    std::unordered_map<std::string, Entity> tagToEntityMap;
    std::unordered_map<int, std::string> entityToTagMap;

    std::unordered_map<std::string, std::set<Entity>> groupToEntitiesMap;
    std::unordered_map<int, std::string> entityToGroupMap;

    public:
    WorldStorage* storage;
    World();
    Entity CreateEntity();
    Entity GetEntity(int entityId);
    void KillEntity(Entity& entity);
    void Update();

    std::vector<Entity> GetEntities();

    template <typename TComponent>
    std::vector<Entity> GetComponentEntities();
    // Tag Management
    void TagEntity(Entity entity, const std::string& tag);
    bool EntityHasTag(Entity entity, const std::string& tag) const;
    Entity GetEntityByTag(const std::string& tag);
    void RemoveEntityTag(Entity entity);

    // Group Management
    void GroupEntity(Entity entity, const std::string& group);
    bool EntityIsInGroup(Entity entity, const std::string& group) const;
    std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
    void RemoveEntityGroup(Entity entity);


    // System templates
    template <typename TSystem, typename... TArgs>
    void AddSystem(TArgs&&... args);
    template <typename TSystem>
    void RemoveSystem();
    template <typename TSystem>
    bool HasSystem() const;
    template <typename TSystem>
    TSystem& GetSystem() const;

    // Entity templates
    void AddEntityToSystems(Entity entity);
    void RemoveEntityFromSystems(Entity entity);
    ~World() = default;
};


template <typename TComponent>
std::vector<Entity> World::GetComponentEntities() {
    std::vector<Entity> entityList;
    const int componentId = Component<TComponent>::GetId();

    for (int entityId = 0; entityId < storage->entityComponentRegistry.size(); entityId++) {
        bool entityIsActive = std::find(freeIds.begin(), freeIds.end(), entityId) == freeIds.end();
        if (!entityIsActive)
            continue;

        bool hasComponent = storage->entityComponentRegistry[entityId].test(componentId);
        if (!hasComponent)
            continue;
        Entity entity = Entity(entityId);
        entity.SetOwner(this);
        entityList.push_back(entity);
    }

    return entityList;
}

template <typename TSystem, typename... TArgs>
void World::AddSystem(TArgs&&... args) {
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    storage->systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void World::RemoveSystem() {
    auto system = storage->systems.find(std::type_index(typeid(TSystem)));
    storage->systems.erase(system);
}

template <typename TSystem>
bool World::HasSystem() const {
    return storage->systems.find(std::type_index(typeid(TSystem))) != storage->systems.end();
}

template <typename TSystem>
TSystem& World::GetSystem() const {
    auto system = storage->systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}
#endif
