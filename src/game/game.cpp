#include "game.h"
#include "../common/common.h"
#include "../editor/editor.h"
#include "../io/cursor.h"
#include "../io/keyboard.h"
#include "../log/log.h"
#include "../scene/scene.h"
#include "../scripting/lua_manager.h"
#include "raylib.h"
#include "../io/filesystem.h"
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

    std::vector<std::string> files = FileSys::GetFilesInDirectory("./assets/tile_maps");

    for(auto file : files)
    {
      if(FileSys::GetFileExtension(file) != "png")
        continue;

      AssetManager::AddTexture(FileSys::GetFileName(file), file);
    }

    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/pre_load");
    Log::Warn("Engine is starting");

    Physics::Initialize(Scene::ecs);
    Log::Warn("Physics Initialized");
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/after_load");

    flecs::entity obj = Scene::CreateEntity("door_tile2");
    selectedTextureId = AssetManager::GetTexture("desert");

    obj.set<RectTransformC>({ glm::vec2(50, 28), glm::vec2(100, 56.25) });
    obj.set<Sprite>({ selectedTextureId, 20, 0, 0, 1920, 1080 });

    selectedTextureId = AssetManager::GetTexture("box");
    flecs::entity floor = Scene::CreateEntity("floor");
    floor.set<RectTransformC>({ glm::vec2(50, 40), glm::vec2(2, 10), glm::vec2(1, 1), 90 });
    floor.set<Sprite>({ selectedTextureId, 20, 0, 0, 64, 64 });
    floor.set<RigidBody>({ true, 0.25, true });
}

void Game::Update() {
    int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        sleepProgram(timeToWait / 4);
    }

    tickLastFrame = getTime();

    Scene::Update();
    DrawFPS(100, 100);


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
