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


int selectedTextureId;
void Game::Setup() {
    // Setup Systems
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/pre_load");
    Log::Warn("Engine is starting");

    // Setup Assets
    Physics::Initialize();
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/after_load");

    static const std::vector<std::string>& loadedAssets = AssetManager::GetLoadedTextures();

    auto obj = Scene::CreateEntity("door_tile2");
    selectedTextureId = AssetManager::GetTexture("desert");
    auto inf = CommancheRenderer::Instance->GetTextureInfo(selectedTextureId);

    obj.set<RectTransform>({glm::vec2(1920, 1080), glm::vec2(1920 * 2, 1080 * 2), glm::vec2(500, 500)});
    obj.set<Sprite>({selectedTextureId, 20, 0, 0, (float)inf.width, (float)inf.height});
}

void Game::Update() {
    int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        sleepProgram(timeToWait);
    }

    CommancheRenderer::Instance->DrawRectRangle(selectedTextureId, 100, 100, 1000, 1000, 0, 0, 0);
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
