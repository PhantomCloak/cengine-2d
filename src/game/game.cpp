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
    // auto inf = CommancheRenderer::Instance->GetTextureInfo(selectedTextureId);

    obj.set<RectTransformC>({ glm::vec2(1920, 1080), glm::vec2(1920 * 2, 1080 * 2), glm::vec2(500, 500), 0 });
    obj.set<Sprite>({ selectedTextureId, 20, 0, 0, 1920, 1080 });
}

void Game::Update() {
    // int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);

    // CommancheRenderer::Instance->DrawRectRangle(100, 100, 50, 50, 0);
    // if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
    //     sleepProgram(timeToWait);
    // }

    // tickLastFrame = getTime();
    //std::cout << "FPS: " << GetFPS() << std::endl;

    Scene::Update();
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
