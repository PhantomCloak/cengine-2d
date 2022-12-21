#include "world.h"
#include "entity.h"

World::World() {
    storage = new WorldStorage();
    eventBus = std::make_shared<EventBus>();
}

Entity World::CreateEntity() {
    int entityId;

    if (freeIds.empty()) {
        entityId = storage->entityNumberCtx++;
        if (storage->entityNumberCtx >= storage->entityComponentRegistry.size()) {
            storage->entityComponentRegistry.resize(entityId + 4);
        }
    } else {
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    Entity entity(entityId);
    entity.SetOwner(this);
    entityAddQueue.insert(entity);

    if (entityId >= storage->entityComponentRegistry.size())
        storage->entityComponentRegistry.resize(entityId + 4);

    return entity;
}

std::vector<Entity> World::GetEntitiesByGroup(const std::string& group) const {

    if (groupToEntitiesMap.find(group) == groupToEntitiesMap.end()) {
      return {};
    }

    auto& setOfEntities = groupToEntitiesMap.at(group);
    return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void World::KillEntity(Entity& entity) {
    entityRemoveQueue.insert(entity);
}

Entity World::GetEntity(int entityId){
  return Entity(entityId);
};

void World::AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();
    const auto& entityEnabledComponents = storage->entityComponentRegistry[entityId];

    for (auto& system : storage->systems) {
        const auto& systemEnabledComponents = system.second->GetEnabledComponents();
        bool isInterested = (entityEnabledComponents & systemEnabledComponents) == systemEnabledComponents;

        if (isInterested) {
            system.second->AddEntity(entity);
        }
    }
}

void World::RemoveEntityFromSystems(Entity entity) {
    for (auto system : storage->systems) {
        system.second->RemoveEntity(entity.GetId());
    }
}

void World::TagEntity(Entity entity, const std::string& tag) {
    tagToEntityMap.emplace(tag, entity);
    entityToTagMap.emplace(entity.GetId(), tag);
}

void World::RemoveEntityTag(Entity entity) {
    auto taggedEntity = entityToTagMap.find(entity.GetId());
    if (taggedEntity == entityToTagMap.end())
        return;
    auto tag = taggedEntity->second;
    tagToEntityMap.erase(tag);
    entityToTagMap.erase(taggedEntity);
}

bool World::EntityHasTag(Entity entity, const std::string& tag) const {
    if (entityToTagMap.find(entity.GetId()) == entityToTagMap.end()) {
        return false;
    }
    return true;
}

void World::GroupEntity(Entity entity, const std::string& group) {
    groupToEntitiesMap.emplace(group, std::set<Entity>());
    groupToEntitiesMap[group].emplace(entity);
    entityToGroupMap.emplace(entity.GetId(), group);
}

void World::RemoveEntityGroup(Entity entity) {
    auto entityGroupNameIterator = entityToGroupMap.find(entity.GetId());
    if (entityGroupNameIterator == entityToGroupMap.end()) {
        Log::Warn("Entity " + std::to_string(entity.GetId()) + " is not part of any group! remove operation has failed. ");
        return;
    }

    std::string& groupName = entityGroupNameIterator->second;

    auto entitiesInGroupIterator = groupToEntitiesMap.find(groupName);
    if (entitiesInGroupIterator == groupToEntitiesMap.end()) {
        Log::Err("Entity group aligment is corrupt");
        return;
    }

    std::set<Entity>& entityList = entitiesInGroupIterator->second;

    auto entityInGroupIterator = entityList.find(entity);
    if (entityInGroupIterator == entityList.end()) {
        Log::Err("Entity set group aligment is corrupt");
        return;
    }

    entityList.erase(entityInGroupIterator);
    entityToGroupMap.erase(entityGroupNameIterator);
}

bool World::EntityIsInGroup(Entity entity, const std::string& group) const {

    auto groupToEntitiesIterator = groupToEntitiesMap.find(group);
    if (groupToEntitiesIterator == groupToEntitiesMap.end()) {
        return false;
    }

    auto entityIterator = groupToEntitiesMap.at(group);
    return entityIterator.find(entity.GetId()) != entityIterator.end();
}

void World::Update() {
    for (auto entity : entityAddQueue) {
        AddEntityToSystems(entity);
    }
    entityAddQueue.clear();

    for (auto entity : entityRemoveQueue) {
        RemoveEntityFromSystems(entity);
        RemoveEntityGroup(entity);
        RemoveEntityTag(entity);
        storage->entityComponentRegistry[entity.GetId()].reset();
        freeIds.push_back(entity.GetId());
    }

    entityRemoveQueue.clear();
}
