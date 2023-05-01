#include "game.h"
#include "../common/common.h"
#include "../editor/editor.h"
#include "../io/cursor.h"
#include "../io/keyboard.h"
#include "../log/log.h"
#include "../scene/scene.h"
#include "../scripting/lua_manager.h"
#include "wapper.h"
#include <filesystem>
#include <iostream>

Game::Game() {
    isRunning = false;
    Scene::Init();


    Log::Inf("Game Constructor Called");
}

Game::~Game() {
    Log::Inf("Game Deconstructor Called");
}


void Game::Initialize() {
    isRunning = true;
}


void Game::Setup() {
    // Setup Systems
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/pre_load");
    Log::Warn("Engine is starting");

    // Setup Assets
    Physics::Initialize();
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/after_load");

//    float w = (3840.0f / 100);
//    float h = (2160.0f / 100);
//
//    float x = w / 2;
//    float y = h / 2;
//    Entity bg = Scene::CreateEntity();
//    bg.AddComponent<RectTransform>(glm::vec2(x, y), glm::vec2(w * 2, h * 2));
//    bg.AddComponent<Sprite>(AssetManager::GetTexture("desert"), -1);
//
//    Entity text = Scene::CreateEntity();
//    text.AddComponent<Label>(glm::vec2(60, 200), "HELLO WORLD");
//
//    Entity player = Scene::CreateEntity();
//    player.AddComponent<RectTransform>(glm::vec2(10, 5), glm::vec2(0.5f, 0.5f));
//    player.AddComponent<Sprite>(AssetManager::GetTexture("box"));
//    player.AddComponent<CharacterController>(10, 0.1f, 0.1f, 0.1f);
//    player.AddComponent<Health>(100);
//    player.AddComponent<RigidBody>(false, 0.0f, false);
//
//
//    Entity platform = Scene::CreateEntity();
//    platform.AddComponent<RectTransform>(glm::vec2(10, 20), glm::vec2(10, 1));
//    platform.AddComponent<Sprite>(AssetManager::GetTexture("box2"));
//    platform.AddComponent<RigidBody>(true, 0, true, glm::vec2(0, 0));
//
//    Scene::SetSystemStatus<RenderSystem>(true);
//    Scene::SetSystemStatus<RenderText2D>(true);
//    Scene::SetSystemStatus<RenderDebug>(true);
//    Scene::SetSystemStatus<PhysicsController>(true);
}

void Game::Update() {
    int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        sleepProgram(timeToWait);
    }

    tickLastFrame = getTime();

    Scene::Update();

    // bus->ClearEvents();
    Keyboard::FlushPressedKeys();
}

void Game::ProcessInput() {
    Keyboard::Poll();
}


void Game::Render() {
    Scene::Render();
}


void Game::Destroy() {
    Scene::Destroy();
    isRunning = false;
}
