#ifndef ENTITY_H
#define ENTITY_H

#include "wstorage.h"
#include "component.h"
#include "../log/log.h"
#include <utility>

class World;

class Entity{
  private:
    int id;
    WorldStorage* owner;
  public:
    Entity(int id) : id(id) {};
    int GetId() const;

    Entity& operator =(const Entity& other) = default;
    bool operator ==(const Entity& other) const { return id == other.id; }
    bool operator !=(const Entity& other) const { return id != other.id; }
    bool operator >(const Entity& other) const { return id > other.id; }
    bool operator <(const Entity& other) const { return id < other.id; }

    template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
    template <typename TComponent> void RemoveComponent();
    template <typename TComponent> bool HasComponent() const;
    template <typename TComponent> TComponent& GetComponent() const;
    void Kill();
    void SetOwner(World * world);

    void Tag(const std::string& tag);
    bool HasTag(const std::string& tag) const;
    void Group(const std::string& group);
    bool IsInGroup(const std::string& group) const;
    
};

template <typename TComponent, typename ...TArgs> 
void Entity::AddComponent(TArgs&& ...args)
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = GetId();

    // check pool needs to be resized
    if (componentId >= owner->componentPools.size()) {
      owner->componentPools.resize(componentId + 4, nullptr);
    }

    // does type pool exist?
    if (!owner->componentPools[componentId]) {
        std::shared_ptr<Pool<TComponent>> newPool = std::make_shared<Pool<TComponent>>();
        owner->componentPools[componentId] = newPool;
    }

    // get pool
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(owner->componentPools[componentId]);

    // does pool needs resized?
    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(owner->entityNumberCtx);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    // asign newly created component the same-type registry
    componentPool->Set(entityId, newComponent);

    // set bitmask for selected entity
    owner->entityComponentRegistry[entityId].set(componentId);
}

template <typename TComponent> void Entity::RemoveComponent()
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = GetId();

    owner->entityComponentRegistry[entityId].set(componentId, false);
}

template <typename TComponent> bool Entity::HasComponent() const
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = GetId();
    return owner->entityComponentRegistry[entityId].test(componentId);

}

template <typename TComponent> TComponent& Entity::GetComponent() const
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = GetId();

    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(owner->componentPools[componentId]);
    
    return componentPool->Get(entityId);
}

#endif
