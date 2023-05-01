#pragma once
#include "../core/util.h"
#include "../ecs/world.h"
#include "../game/commanche2d.h"
#include "glm/glm.hpp"
#include <iostream>
#include <sol/sol.hpp>
#include <string>

#if EDITOR
#include "../editor/editor.h"
#endif

class Scene {
    public:
    // static Scene LoadScene(std::string sceneName);
    // static void RegisterCallback(void (*onSceneLoaded)(std::string sceneName));

    static void Init();
    static Entity CreateEntity();
    static void DestroyEntity(Entity e);
    static std::vector<Entity> GetEntities();

    template <typename TSystem, typename... TArgs>
    static void LoadSystem(TArgs&&... args);
    template <typename TSystem, typename... TArgs>
    static void AddSystem(TArgs&&... args);
    template <typename TSystem>
    static void SetSystemStatus(bool isActive);
    static void SetSystemStatus(std::string systemId, bool isActive);
    template <typename TSystem>
    static void RemoveSystem();
    template <typename TSystem>
    static bool HasSystem();
    template <typename TSystem>
    static TSystem& GetSystem();

    template <typename TComponent, typename... TArgs>
    static void AddComponentToEntity(Entity entity, TArgs&&... args);

    static void Update();
    static void Render();

    static void Destroy();

    static World* world;

    private:
    static std::string currentScenePath;
    static CommancheRenderer* renderer;
#if EDITOR
    static Editor* editor;
#endif
};

template <typename TSystem, typename... TArgs>
void Scene::AddSystem(TArgs&&... args) {
    world->AddSystem<TSystem>(args...);
}

template <typename TSystem, typename... TArgs>
void Scene::LoadSystem(TArgs&&... args) {
    world->LoadSystem<TSystem>(args...);
}

template <typename TSystem>
void Scene::SetSystemStatus(bool isActive) {
    auto& system = world->storage->systems[typeid(TSystem)];
    system->isActive = isActive;
}

inline void Scene::SetSystemStatus(std::string systemId, bool isActive) {
    for (auto& systemEntry : world->storage->systems) {
        if (strcmp(systemEntry.first.name(), systemId.c_str()) == 0) {
            systemEntry.second->isActive = isActive;
            return;
        }
    }
}

template <typename TComponent, typename... TArgs>
void Scene::AddComponentToEntity(Entity entity, TArgs&&... args) {
    world->GetEntity(entity.GetId()).AddComponent<TComponent>(args...);
    world->AddEntityToSystems(entity);
}

template <typename TSystem>
void Scene::RemoveSystem() {
    world->RemoveSystem<TSystem>();
}

template <typename TSystem>
bool Scene::HasSystem() {
    return world->HasSystem<TSystem>();
}

template <typename TSystem>
TSystem& Scene::GetSystem() {
    return world->GetSystem<TSystem>();
}
