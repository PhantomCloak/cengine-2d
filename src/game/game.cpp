#include "game.h"
#include "../log/log.h"
#include "../scripting/lua_manager.h"
#include "wapper.h"
#include "../common/common.h"
#include <filesystem>
#include <iostream>

int Game::mapHeight = 0;
int Game::mapWidth = 0;
Wrapper* wrapper;
glm::vec2 Game::cursor;

Game::Game() {
    isRunning = false;
    world = std::make_unique<World>();
    cursor = glm::vec2(0);
    renderer = new CommancheRenderer();
    //editor = new Editor();
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

    //Entity text = world->CreateEntity();
    //const CommancheColorRGB& color = { 255, 255, 255 };
    //text.AddComponent<Health>();
    //text.AddComponent<Label>(glm::vec2(1920 / 2 - 120, 200), "CENGINE-2D", "charriot-font", color);


    Entity player = world->CreateEntity();
    player.AddComponent<RectTransform>(glm::vec2(40, 40), glm::vec2(1, 1), glm::vec2(1, 1), 0);
    player.AddComponent<Sprite>(AssetManager::GetTexture("box"), 10, 10);
    //player.AddComponent<CharacterController>(5,1,1,1);
    player.AddComponent<Mesh>();
    player.AddComponent<RigidBody>(false);


    //Entity platform = world->CreateEntity();
    //platform.AddComponent<RectTransform>(glm::vec2(2, 10), glm::vec2(4, 1), glm::vec2(1, 1), 0);
    //platform.AddComponent<Sprite>(AssetManager::GetTexture("box2"), 100, 32, 1);
    //platform.AddComponent<Mesh>();
    //platform.AddComponent<RigidBody>(true, 0, glm::vec2(0, 0));


    //Entity platform2 = world->CreateEntity();
    //platform2.AddComponent<RectTransform>(glm::vec2(14, 10), glm::vec2(4, 1), glm::vec2(1, 1), 0);
    //platform2.AddComponent<Sprite>(AssetManager::GetTexture("box2"), 100, 32, 1);
    //platform2.AddComponent<Mesh>();
    //platform2.AddComponent<RigidBody>(true, 0, glm::vec2(0, 0));
}
void Game::Update() {
    int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        sleepProgram(timeToWait);
    }

    float dt = (getTime() - tickLastFrame) / 1000.0f;
    tickLastFrame = getTime();

    world->Update();

    //world->GetSystem<CharacterSystem>().Update();
    //world->GetSystem<MovementSystem>().Update(dt);
    //world->GetSystem<Animator>().Update();
    //world->GetSystem<Physics>().Update();
    bus->ClearEvents();
}

void Game::ProcessInput() {
    int cx = 0;
    int cy = 0;

    //SDL_GetMouseState(&cx, &cy);
    cursor = glm::vec2(cx, cy);

    //SDL_PumpEvents();

    int keyArraySize = 0;
   // const uint8_t* keyArray = SDL_GetKeyboardState(&keyArraySize);

   // if (keyArraySize > 0) {
   //     bus->PushEvent<KeyPressEvent>(keyArray);
   // }

   // if (keyArray[SDL_SCANCODE_ESCAPE]) {
   //     isRunning = false;
   // }

   // SDL_Event event;
   // while (SDL_PollEvent(&event) != 0) {
   //     switch (event.type) {
   //     case SDL_WINDOWEVENT:
   //         if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
   //           glViewport(0, 0, event.window.data1, event.window.data2);
   //         }
   //     case SDL_KEYUP: {
   //         bus->PushEvent<KeyPressUpEvent>(event.key.keysym.sym);
   //     } break;
   //     }
   //     //editor->ProcessInput(&event);
   // }
}


void Game::Render() {
    //world->GetSystem<RenderText2D>().Update();

    //world->GetSystem<RenderDebug>().Update();

    //editor->Render();
    world->GetSystem<RenderSystem>().Update();
    renderer->Render();
}


void Game::Destroy() {
    isRunning = false;
    renderer->Destroy();
}
