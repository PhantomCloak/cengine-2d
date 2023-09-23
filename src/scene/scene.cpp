#include "scene.h"
#include "../ecs/serializer.h"
#include "../game/game.h"
#include "../io/cursor.h"
#include "../scripting/lua_manager.h"

World* Scene::world = nullptr;
CommancheRenderer* Scene::renderer = nullptr;
std::string Scene::currentScenePath = "";
#if EDITOR
Editor* Scene::editor = nullptr;
#endif


void Scene::Init() {
    world = new World();

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


    // Add All systems at once
    Scene::LoadSystem<RenderSystem>(renderer);
    Scene::LoadSystem<RenderText2D>(renderer);
    Scene::LoadSystem<RenderDebug>(renderer);
    Scene::LoadSystem<PhysicsController>();

    Keyboard::Setup(renderer->wnd);
    Cursor::Setup(renderer->wnd);

#if EDITOR
    editor = new Editor();
    editor->Init(renderer);
#endif
}

Entity Scene::CreateEntity() {
    return world->CreateEntity();
}

void Scene::DestroyEntity(Entity entity) {
    // world->De
}

void Scene::Update() {
    world->Update();
}

void Scene::Destroy() {
    renderer->Destroy();
}

void Scene::Render() {
    renderer->RenderStart();
    Scene::GetSystem<RenderSystem>().Update();
    Scene::GetSystem<RenderText2D>().Update();
    //Scene::GetSystem<RenderDebug>().Update();
    Scene::GetSystem<PhysicsController>().Update();
    renderer->RenderEnd();

#if EDITOR
    editor->Render();
#endif

    renderer->RenderApply();
}

std::vector<Entity> Scene::GetEntities() {
    return world->GetEntities();
}
