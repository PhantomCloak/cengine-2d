#ifndef SYSTEM_H
#define SYSTEM_H

#include "entity.h"
#include "component.h"
#include <bitset>
#include <vector>

const int MAX_COMPONENTS = 128;
typedef std::bitset<MAX_COMPONENTS> ComponentRegistry;

class System {
    private:
    ComponentRegistry componentsEnabled;
    std::vector<Entity> entities;

    public:
    System() = default;
    ~System() = default;

    void AddEntity(Entity entity);
    void RemoveEntity(int id);
    std::vector<Entity>& GetEntities() const;
    const ComponentRegistry& GetEnabledComponents() const;
    template <typename T> void RequireComponent();
};

template <typename T> void System::RequireComponent(){
  const auto componentId = Component<T>::GetId();
  componentsEnabled.set(componentId);
};
#endif
