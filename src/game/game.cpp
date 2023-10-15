#include "game.h"
#include "../assetmgr/AssetManager.h"
#include "../common/common.h"
#include "../io/cursor.h"
#include "../io/filesystem.h"
#include "../io/keyboard.h"
#include "../log/log.h"
#include "../physics/physics.h"
#include "../scene/scene.h"
#include "../scripting/lua_manager.h"

#include "../components/Sprite.h"

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
    //std::vector<std::string> files = FileSys::GetFilesInDirectory("./assets/tile_maps");

    //std::cout << "Size of Sprite: " << sizeof(Sprite) << std::endl;

    //for (auto file : files) {
    //    if (FileSys::GetFileExtension(file) != "png")
    //        continue;

    //    AssetManager::AddTexture(FileSys::GetFileName(file), file);
    //}

    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/pre_load");
    Log::Warn("Engine is starting");

    Physics::Initialize(Scene::ecs);
    Log::Warn("Physics Initialized");
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/after_load");

    flecs::entity obj = Scene::CreateEntity("door_tile2");
    ////selectedTextureId = AssetManager::GetTexture("desert");

    obj.set<RectTransformC>({ glm::vec2(50, 28), glm::vec2(500, 500) });
    obj.set<Sprite>({ "desert", 20, 0, 0, 1920, 1080 });

    //selectedTextureId = AssetManager::GetTexture("box");
    //flecs::entity floor = Scene::CreateEntity("floor");
    //floor.set<RectTransformC>({ glm::vec2(50, 40), glm::vec2(2, 10), glm::vec2(1, 1), 90 });
    //floor.set<Sprite>({ "box", 21, 0, 0, 64, 64 });
    //floor.set<RigidBody>({ true, 0.25, true });


    //flecs::entity floor2 = Scene::CreateEntity("floor2");
    //floor2.set<RectTransformC>({ glm::vec2(60, 30), glm::vec2(2, 10), glm::vec2(1, 1), 0 });
    //floor2.set<Sprite>({ "box", 22, 0, 0, 64, 64 });
    //floor2.set<RigidBody>({ true, 0.25, true });
}

void Game::Update() {
    int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        //sleepProgram(timeToWait / 4);
    }

    tickLastFrame = getTime();

    if(Keyboard::IsKeyPressed(KEY_D))
    {

    }

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
