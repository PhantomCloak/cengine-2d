#include "scene.h"
#include "../ecs/serializer.h"
#include "../game/game.h"
#include "../io/cursor.h"
#include "../scripting/lua_manager.h"
#include "../systems/systems.h"

CommancheRenderer* Scene::renderer = nullptr;
std::string Scene::currentScenePath = "";

flecs::world Scene::ecs;

Editor* Scene::editor = nullptr;

void Scene::Init() {
    Systems::Init(ecs);
    editor = new Editor();
    editor->Init(renderer);

    renderer = new CommancheRenderer();

    AssetManager::Initialize(renderer);
    LuaManager::InitSandbox();

    if (currentScenePath.size() <= 0) {
        Log::Inf("No active scene deteced creating new scene on assets/default.json");
        currentScenePath = "assets/default.json";
    }

    LuaManager::RegisterCppToLuaFunc("addTexture", &AssetManager::AddTexture);

    LuaManager::LoadLuaFile("./assets/scripts/config.lua");
    sol::table initCfg = LuaManager::LoadLuaTable("config");

    sol::table displayCfg = LuaManager::LoadLuaTable("config");

    int screenW = displayCfg["resolution"]["width"];
    int screenH = displayCfg["resolution"]["height"];

    renderer->Initialize("Twelve Villages", screenW, screenH);
    renderer->InitializeShaders("./src/shaders");

    Keyboard::Setup(renderer->wnd);
    Cursor::Setup(renderer->wnd);
}

flecs::entity Scene::CreateEntity(std::string entityName) {
    return ecs.entity(entityName.c_str());
}

void Scene::DestroyEntity(flecs::entity entity) {
    entity.destruct();
}

void Scene::Update() {
    ecs.progress();
}

void Scene::Destroy() {
    renderer->Destroy();
}

void Scene::Render() {
  editor->Render();
}

std::vector<flecs::entity> Scene::GetEntities() {
    std::vector<flecs::entity> entities;
    ecs.each([&](flecs::entity e, RectTransformC& transform) {
        entities.push_back(e);
    });
    return entities;
}
