#include "game.h"
#include "../log/log.h"
#include "../scripting/lua_manager.h"
#include "wapper.h"
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
    editor = new Editor();
    AssetManager::Initialize(renderer);
    Log::Inf("Game Constructor Called");

    wrapper = new Wrapper(world);

    LuaManager::InitSandbox();

    LuaManager::RegisterCppToLuaFunc("addTile", &Map::PlaceTile, map);
    LuaManager::RegisterCppToLuaFunc("getEntityFromTileIndex", &Map::GetEntityFromTileIndex, map);
    LuaManager::RegisterCppToLuaFunc("addCollider", &Wrapper::AddCollider, wrapper);

    LuaManager::LoadLuaFile("./assets/scripts/config.lua");
    sol::table initCfg = LuaManager::LoadLuaTable("config");
    this->map = new Map(world, initCfg["map"]["tile_size"], initCfg["map"]["tile_scale"]);
}

Game::~Game() {
    Log::Inf("Game Deconstructor Called");
}


void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error: %s\n", SDL_GetError());
    }

    sol::table displayCfg = LuaManager::LoadLuaTable("config");

    int screenW = displayCfg["resolution"]["width"];
    int screenH = displayCfg["resolution"]["height"];

    renderer->Initialize("Twelve Villages", screenW, screenH);
    editor->Init(renderer, map, world, &camera, world->eventBus);

    isRunning = true;

    camera.x = 0;
    camera.y = 0;
    camera.w = screenW;
    camera.h = screenH;
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
    world->AddSystem<CollisionResolver>(world->eventBus);
    world->AddSystem<DamageSystem>(world->eventBus);
    world->AddSystem<CharacterSystem>(world->eventBus);
    world->AddSystem<CameraSystem>(&camera, map);
    world->AddSystem<ProjectileSystem>();
    world->AddSystem<ProjectileEmitterSystem>(world, world->eventBus);

    // Setup Entities
    Entity chopper = world->CreateEntity();
    chopper.Tag("player");

    chopper.AddComponent<Transform>(glm::vec2(10, 30), glm::vec2(2, 2), 0);
    chopper.AddComponent<RigidBody>(glm::vec2(0, 0));
    chopper.AddComponent<Sprite>(AssetManager::GetTexture("chopper"), 32, 32, 1);
    chopper.AddComponent<Animation>(2, 25, true);
    chopper.AddComponent<Camera>();
    //chopper.AddComponent<Health>(100);
    chopper.AddComponent<BoxCollider>(32, 32);
    chopper.AddComponent<CharacterController>(80);
    chopper.AddComponent<ProjectileEmitter>(true, glm::vec2(200, 200), 10000, 1500, 10);

    Entity text = world->CreateEntity();
    const SDL_Color& color = { 255, 255, 255 };
    text.AddComponent<Health>();
    text.AddComponent<Label>(glm::vec2(1920 / 2 - 120, 200), "CENGINE-2D", "charriot-font", color);

    std::vector<Entity> ent = world->GetComponentEntities<Health>();
}

void Game::Update() {
    int timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - tickLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        SDL_Delay(timeToWait);
    }

    float dt = (SDL_GetTicks() - tickLastFrame) / 1000.0f;
    tickLastFrame = SDL_GetTicks();

    world->Update();

    world->GetSystem<MovementSystem>().Update(dt);
    world->GetSystem<Animator>().Update();
    world->GetSystem<CollisionResolver>().Update();
    world->GetSystem<ProjectileEmitterSystem>().Update();
    world->GetSystem<CameraSystem>().Update();
    world->GetSystem<ProjectileSystem>().Update();
}

void Game::ProcessInput() {
    SDL_Event event;
    int cx = 0;
    int cy = 0;

    SDL_GetMouseState(&cx, &cy);
    cursor = glm::vec2(cx, cy);
    while (SDL_PollEvent(&event)) {
        editor->ProcessInput(&event);
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                isRunning = false;
            if (event.key.keysym.sym == SDLK_d)
                isDebug = !isDebug;
            world->eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            world->eventBus->EmitEvent<MousePressedEvent>(event.key.keysym.sym);
            break;
        case SDL_KEYUP:

            break;
        }
    }
}


void Game::Render() {
    world->GetSystem<RenderSystem>().Update();
    world->GetSystem<RenderText2D>().Update();

    if (isDebug)
        world->GetSystem<RenderDebug>().Update();

    editor->Render();
    renderer->Render();
}


void Game::Destroy() {
    isRunning = false;
    renderer->Destroy();
}
