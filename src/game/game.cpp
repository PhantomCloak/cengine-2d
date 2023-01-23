#include "game.h"
#include "../common/common.h"
#include "../core/cursor.h"
#include "../core/keyboard.h"
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
    // editor = new Editor();
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
    AssetManager::AddFont("charriot-font", "/Users/unalozyurt/Workspace/commanche-2d/assets/fonts/charriot.ttf", 64);

    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/after_load");

    world->AddSystem<MovementSystem>();
    world->AddSystem<RenderSystem>(renderer, &camera);
    world->AddSystem<RenderText2D>(renderer, &camera);
    world->AddSystem<RenderDebug>(renderer, &camera);
    world->AddSystem<Animator>();
    world->AddSystem<Physics>();
    world->AddSystem<CharacterSystem>(bus, &camera);
    world->AddSystem<ProjectileSystem>();

    Entity bg = world->CreateEntity();
    bg.AddComponent<RectTransform>(glm::vec2(100, 60), glm::vec2(200, 120));
    bg.AddComponent<Sprite>(AssetManager::GetTexture("desert"), -1);

    Entity player = world->CreateEntity();
    player.AddComponent<RectTransform>(glm::vec2(100, 10), glm::vec2(5, 5));
    player.AddComponent<Sprite>(AssetManager::GetTexture("box"));
    player.AddComponent<CharacterController>(8,0.5f,0.5f,0.5f);
    player.AddComponent<RigidBody>(false);


     Entity platform = world->CreateEntity();
     platform.AddComponent<RectTransform>(glm::vec2(100, 90), glm::vec2(100, 5));
     platform.AddComponent<Sprite>(AssetManager::GetTexture("box2"));
     platform.AddComponent<RigidBody>(true, 0, glm::vec2(0, 0));
}
void Game::Update() {
    int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        sleepProgram(timeToWait);
    }

    float dt = (getTime() - tickLastFrame) / 1000.0f;
    tickLastFrame = getTime();

    world->Update();

     world->GetSystem<CharacterSystem>().Update();
     world->GetSystem<Animator>().Update();
     world->GetSystem<Physics>().Update();

    bus->ClearEvents();
    Keyboard::FlushPressedKeys();
}

void Game::ProcessInput() {
    Keyboard::Poll();
}


void Game::Render() {
    //editor->Render();
    world->GetSystem<RenderSystem>().Update();
    renderer->Render();
}


void Game::Destroy() {
    isRunning = false;
    renderer->Destroy();
}
