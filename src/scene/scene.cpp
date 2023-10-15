#include "scene.h"
#include "../assetmgr/AssetManager.h"
#include "../ecs/serializer.h"
#include "../editor/editor.h"
#include "../game/game.h"
#include "../io/cursor.h"
#include "../io/keyboard.h"
#include "../scripting/lua_manager.h"
#include "../systems/systems.h"

CommancheRenderer* Scene::renderer = nullptr;
std::string Scene::currentScenePath = "";

flecs::world Scene::ecs;
#if EDITOR
Editor* editor = nullptr;
#endif

void Scene::Init() {
    renderer = new CommancheRenderer();
    AssetManager::Initialize(renderer);
    Systems::Init(ecs);
    LuaManager::InitSandbox();
    if (currentScenePath.size() <= 0) {
        Log::Inf("No active scene deteced creating new scene on assets/default.json");
        currentScenePath = "assets/default.json";
    }

    LuaManager::RegisterCppToLuaFunc("addTexture", &AssetManager::AddTexture);

    int screenW = 1920;
    int screenH = 1080;

    renderer->Initialize("Twelve Villages", screenW, screenH);
    renderer->InitializeShaders("./src/shaders");

#if EDITOR
    renderer->isTextureModeEnabled = false;
    editor = new Editor();
    editor->Init(renderer);
#endif

    Keyboard::Setup();
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
#if EDITOR
    editor->Render();
#endif
}

std::vector<flecs::entity> Scene::GetEntities() {
    std::vector<flecs::entity> entities;
    static auto q = ecs.query_builder<RectTransformC>()
                    .order_by(0, [](flecs::entity_t e1, const void* ptr1, flecs::entity_t e2, const void* ptr2) {
                        return (e1 > e2) - (e1 < e2);
                    })
                    .build();

    q.each([&](const flecs::entity& e, RectTransformC& transform) {
        entities.push_back(e);
    });

    return entities;
}
