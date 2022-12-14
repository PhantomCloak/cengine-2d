#include "system.h"

void System::AddEntity(Entity entity) {
    entities.push_back(entity);
}

void System::RemoveEntity(int entity) {
    auto itResult = std::find(entities.begin(), entities.end(), entity);
    if (itResult == entities.end())
        return;

    entities.erase(itResult);
}

std::vector<Entity>& System::GetEntities() const {
    return const_cast<std::vector<Entity>&>(entities);
}

const ComponentRegistry& System::GetEnabledComponents() const {
    return componentsEnabled;
}
