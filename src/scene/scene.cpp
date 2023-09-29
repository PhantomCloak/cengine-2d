#include "scene.h"
#include "../ecs/serializer.h"
#include "../game/game.h"
#include "../io/cursor.h"
#include "../scripting/lua_manager.h"
#include "../systems/systems.h"

CommancheRenderer* Scene::renderer = nullptr;
std::string Scene::currentScenePath = "";

flecs::world* Scene::ecs;

#if EDITOR
Editor* Scene::editor = nullptr;
#endif

void Scene::Init() {
    ecs = new flecs::world();

    ecs->system("RenderStart")
    .kind(flecs::PreUpdate)
    .iter([](flecs::iter it) {
        renderer->RenderStart();
    });

    ecs->system("RenderEnd")
    .kind(flecs::PostUpdate)
    .iter([](flecs::iter it) {
        renderer->RenderEnd();
    });
    renderer = new CommancheRenderer();

    AssetManager::Initialize(renderer);
    LuaManager::InitSandbox();

    if (currentScenePath.size() <= 0) {
        Log::Inf("No active scene deteced creating new scene on assets/default.json");
        currentScenePath = "assets/default.json";
        EngineSerializer::SerializeSceneToFile(currentScenePath);
    }

    EngineSerializer::DeserializeFileToScene(currentScenePath);

    // LuaManager::RegisterCppToLuaFunc("addTile", &Map::PlaceTile, map);
    // LuaManager::RegisterCppToLuaFunc("getEntityFromTileIndex", &Map::GetEntityFromTileIndex, map);
    // LuaManager::RegisterCppToLuaFunc("addCollider", &Wrapper::AddCollider, wrapper);

    LuaManager::RegisterCppToLuaFunc("addTexture", &AssetManager::AddTexture);

    LuaManager::LoadLuaFile("./assets/scripts/config.lua");
    sol::table initCfg = LuaManager::LoadLuaTable("config");

    sol::table displayCfg = LuaManager::LoadLuaTable("config");

    int screenW = displayCfg["resolution"]["width"];
    int screenH = displayCfg["resolution"]["height"];

    renderer->Initialize("Twelve Villages", screenW, screenH);
    renderer->InitializeShaders("./src/shaders");

    Systems::Init(ecs);

    // Add All systems at once
    // Scene::LoadSystem<RenderSystem>(renderer);
    // Scene::LoadSystem<RenderText2D>(renderer);
    // Scene::LoadSystem<RenderDebug>(renderer);
    // Scene::LoadSystem<PhysicsController>();
    //

#if EDITOR
    editor = new Editor();
    editor->Init(renderer);
#endif

    Keyboard::Setup(renderer->wnd);
    Cursor::Setup(renderer->wnd);
}

flecs::entity Scene::CreateEntity(std::string entityName) {
    return ecs->entity(entityName.c_str());
}

void Scene::DestroyEntity(flecs::entity entity) {
    entity.destruct();
}

void Scene::Update() {
    ecs->progress();
}

void Scene::Destroy() {
    renderer->Destroy();
}

void Scene::Render() {
    // renderer->RenderStart();
    //  Scene::GetSystem<RenderSystem>().Update();
    //  Scene::GetSystem<RenderText2D>().Update();
    //  Scene::GetSystem<PhysicsController>().Update();
    //  ecs->system<RectTransform, Sprite>("RenderSystem").kind(0).iter(&Systems::RenderSystem);
    //  Scene::Update();
    ////ecs->progress();
    // renderer->RenderEnd();

#if EDITOR
    editor->Render();
#endif

    renderer->RenderApply();
}

std::vector<flecs::entity> Scene::GetEntities() {
    std::vector<flecs::entity> entities;
    ecs->each([&](flecs::entity e, RectTransform& transform) {
        entities.push_back(e);
    });
    return entities;
}
