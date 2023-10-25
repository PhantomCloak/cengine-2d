#include "game.h"
#include "../assetmgr/AssetManager.h"
#include "../common/common.h"
#include "../components/Label.h"
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
    std::vector<std::string> files = FileSys::GetFilesInDirectory("./assets/tile_maps");

    std::cout << "Size of Sprite: " << sizeof(Sprite) << std::endl;

    for (auto file : files) {
        if (FileSys::GetFileExtension(file) != "png")
            continue;

        AssetManager::AddTexture(FileSys::GetFileName(file), file);
    }

    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/pre_load");
    Log::Warn("Engine is starting");

    Physics::Initialize(Scene::ecs);
    Log::Warn("Physics Initialized");
    LuaManager::LoadLuaFilesInDirectory("./assets/scripts/after_load");

    flecs::entity obj = Scene::CreateEntity("door_tile2");
    obj.set<RectTransformC>({ glm::vec2(0, 0), glm::vec2(400, 400) });
    obj.set<Sprite>({ "desert", 20, 0, 0, 1920, 1080 });

    flecs::entity obj2 = Scene::CreateEntity("label");
    obj2.set<RectTransformC>({ glm::vec2(10, 10), glm::vec2(5, 5) });
    obj2.set<Label>({ glm::vec2(10, 10), "Hello World", 20, { 255, 255, 255 }, true });

    flecs::entity cube = Scene::CreateEntity("cube");
    cube.set<RectTransformC>({ glm::vec2(10, 10), glm::vec2(5, 5) });
    cube.set<Sprite>({ "box", 10, 0, 0, 1920, 1080 });

}

void Game::Update() {
    int timeToWait = FRAME_TIME_LENGTH - (getTime() - tickLastFrame);
    static int fps = 0;
    static int fpsLastCheck = 0;
    fps++;
    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
        sleepProgram(timeToWait);
    }

    if ((getTime() - fpsLastCheck) > 1000) {
        printf("FPS: %d\n", fps);
        fps = 0;
        fpsLastCheck = getTime();
    }

    tickLastFrame = getTime();

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
