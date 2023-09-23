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

    static const std::vector<std::string>& loadedAssets = AssetManager::GetLoadedTextures();

    Scene::CreateEntity();
    auto obj = Scene::CreateEntity();
    static int selectedTextureId = AssetManager::GetTexture("dawn_like_door0");

    obj.AddComponent<RectTransform>(glm::vec2(500, 250), glm::vec2(500, 500), glm::vec2(16, 16));
    obj.AddComponent<Sprite>(selectedTextureId, 2000, 0.1f, 0.1f);
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
