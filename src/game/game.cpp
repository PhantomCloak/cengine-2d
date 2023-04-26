#include "game.h"
#include "../common/common.h"
#include "../editor/editor.h"
#include "../io/cursor.h"
#include "../io/keyboard.h"
#include "../log/log.h"
#include "../scripting/lua_manager.h"
#include "wapper.h"
#include <filesystem>
#include <iostream>

int Game::mapHeight = 0;
int Game::mapWidth = 0;
Wrapper* wrapper;

Game::Game() {
    isRunning = false;
    world = std::make_unique<World>();
    renderer = new CommancheRenderer();

#if EDITOR
    editor = new Editor();
#endif

    bus = new EventBus();
    AssetManager::Initialize(renderer);
    Log::Inf("Game Constructor Called");

    wrapper = new Wrapper(world);

    LuaManager::InitSandbox();

    LuaManager::RegisterCppToLuaFunc("addTile", &Map::PlaceTile, map);
    LuaManager::RegisterCppToLuaFunc("getEntityFromTileIndex", &Map::GetEntityFromTileIndex, map);
    LuaManager::RegisterCppToLuaFunc("addCollider", &Wrapper::AddCollider, wrapper);
    LuaManager::RegisterCppToLuaFunc("addTexture", &AssetManager::AddTexture);

    LuaManager::LoadLuaFile("./assets/scripts/config.lua");
    sol::table initCfg = LuaManager::LoadLuaTable("config");
    this->map = new Map(world, initCfg["map"]["tile_size"], initCfg["map"]["tile_scale"]);
}

Game::~Game() {
    Log::Inf("Game Deconstructor Called");
}


void Game::Initialize() {

    sol::table displayCfg = LuaManager::LoadLuaTable("config");

    int screenW = displayCfg["resolution"]["width"];
    int screenH = displayCfg["resolution"]["height"];

    renderer->Initialize("Twelve Villages", screenW, screenH);

    renderer->InitializeShaders("./src/shaders");

#if EDITOR
    editor->Init(renderer, map, world, &camera, bus);
#endif

    Keyboard::Setup(renderer->wnd);
    Cursor::Setup(renderer->wnd);
    isRunning = true;

    camera.x = 0;
    camera.y = 0;
    camera.width = screenW;
    camera.height = screenH;
    camera.gridSize = 64;
}


void Game::Setup() {
    // Setup Systems
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/pre_load");
    Log::Warn("Engine is starting");

    // Setup Assets
    Physics::Initialize();
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/after_load");

    world->AddSystem<MovementSystem>();
    world->AddSystem<RenderSystem>(renderer, &camera);
    world->AddSystem<RenderText2D>(renderer, &camera);
    world->AddSystem<RenderDebug>(renderer);
    world->AddSystem<Animator>();
    world->AddSystem<PhysicsController>();
    world->AddSystem<CharacterSystem>(bus, &camera);
    world->AddSystem<ProjectileSystem>();

    Entity bg = world->CreateEntity();
    float w = (3840.0f / 100);
    float h = (2160.0f / 100);

    float x = w / 2;
    float y = h / 2;

    bg.AddComponent<RectTransform>(glm::vec2(x, y), glm::vec2(w * 2, h * 2));
    bg.AddComponent<Sprite>(AssetManager::GetTexture("desert"), -1);

    Entity text = world->CreateEntity();
    text.AddComponent<Label>(glm::vec2(60, 200), "HELLO WORLD");

    Entity player = world->CreateEntity();
    player.AddComponent<RectTransform>(glm::vec2(10, 5), glm::vec2(0.5f, 0.5f));
    player.AddComponent<Sprite>(AssetManager::GetTexture("box"));
    player.AddComponent<CharacterController>(10, 0.1f, 0.1f, 0.1f);
    player.AddComponent<Health>(100);
    player.AddComponent<RigidBody>(false, 0.0f, false);


    Entity platform = world->CreateEntity();
    platform.AddComponent<RectTransform>(glm::vec2(10, 20), glm::vec2(10, 1));
    platform.AddComponent<Sprite>(AssetManager::GetTexture("box2"));
    platform.AddComponent<RigidBody>(true, 0, true, glm::vec2(0, 0));

//    Entity entity = world->CreateEntity();
//    entity.AddComponent<Sprite>(AssetManager::GetTexture("box"));
//    entity.AddComponent<RectTransform>(glm::vec2(0.8f, 0.5f), glm::vec2(1));
//    entity.AddComponent<DebugTile>();
//
     //for (int i = 0; i < 45; i++)
     //    for (int j = 0; j < 80; j++) {
     //      Entity entity = world->CreateEntity();
     //      static float xo = 0.28f;
     //      static float yo = 0.27f;

     //      float x = 0.5f * j;
     //      float y = 0.5f * i;
     //      entity.AddComponent<Sprite>(AssetManager::GetTexture("box"));
     //      entity.AddComponent<RectTransform>(glm::vec2(xo + x, yo + y), glm::vec2(0.5f, 0.5f));
     //      entity.AddComponent<DebugTile>();
     //    }
}
void Game::Update() {
    int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        sleepProgram(timeToWait);
    }

    tickLastFrame = getTime();

    world->Update();

    world->GetSystem<CharacterSystem>().Update();
    world->GetSystem<Animator>().Update();
    world->GetSystem<PhysicsController>().Update();


    bus->ClearEvents();
    Keyboard::FlushPressedKeys();
}

void Game::ProcessInput() {
    Keyboard::Poll();
}


void Game::Render() {
    renderer->RenderStart();
    world->GetSystem<RenderSystem>().Update();
    world->GetSystem<RenderText2D>().Update();
    world->GetSystem<RenderDebug>().Update();
    renderer->RenderEnd();


#if EDITOR
    editor->Render();
#endif

    renderer->RenderApply();
}


void Game::Destroy() {
    isRunning = false;
    renderer->Destroy();
}
