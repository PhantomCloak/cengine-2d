#include "game.h"
#include "../common/common.h"
#include "../editor/editor.h"
#include "../io/cursor.h"
#include "../io/keyboard.h"
#include "../log/log.h"
#include "../scene/scene.h"
#include "../scripting/lua_manager.h"
#include "raylib.h"
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


int selectedTextureId;
void Game::Setup() {
    // Setup Systems
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/pre_load");
    Log::Warn("Engine is starting");

    Physics::Initialize(Scene::ecs);
    Log::Warn("Physics Initialized");
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/after_load");

    flecs::entity obj = Scene::CreateEntity("door_tile2");
    selectedTextureId = AssetManager::GetTexture("desert");

    obj.set<RectTransformC>({ glm::vec2(1920, 1080), glm::vec2(1920 * 2, 1080 * 2), glm::vec2(500, 500), 0 });
    obj.set<Sprite>({ selectedTextureId, 20, 0, 0, 1920, 1080 });
}


void Game::Update() {
    // Get the current time
    static std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
    static int fps;
    fps++;


    // Update the game scene
    Scene::Update();

    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{ 1 }) {
        oldTime = std::chrono::high_resolution_clock::now();
        printf("Immediate FPS: %d\n", fps);
        std::cout << "FPS: " << fps << std::endl;
        fps = 0;
    }

    // Print FPS
}
// void Game::Update() {
//     int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);
//
//     if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
//          sleepProgram(timeToWait);
//     }
//
//     tickLastFrame = getTime();
//
//     Scene::Update();
//     //DrawFPS(100, 100);
//
//
//     Keyboard::FlushPressedKeys();
// }

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
