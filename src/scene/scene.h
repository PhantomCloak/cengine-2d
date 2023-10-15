#pragma once
#include "glm/glm.hpp"
#include "flecs.h"
#include "../render/render.h"


class Scene {
    public:
    static void Init();
    static flecs::entity CreateEntity(std::string entityName);
    static void DestroyEntity(flecs::entity e);
    static std::vector<flecs::entity> GetEntities();

    template <typename TSystem, typename... TArgs>
    static void AddSystem(TArgs&&... args);
    template <typename TComponent, typename... TArgs>
    static void AddComponentToEntity(flecs::entity entity, TArgs&&... args);

    static void Update();
    static void Render();

    static void Destroy();

    static flecs::world ecs;

    private:
    static std::string currentScenePath;
    static CommancheRenderer* renderer;
};

template <typename TSystem, typename... TArgs>
void Scene::AddSystem(TArgs&&... args) {
  ecs.system<TSystem>(std::forward<TArgs>(args)...);
}

template <typename TComponent, typename... TArgs>
void Scene::AddComponentToEntity(flecs::entity entity, TArgs&&... args) {
    entity.add<TComponent>(std::forward<TArgs>(args)...);
}
