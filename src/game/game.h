#ifndef GAME_H
#define GAME_H

#define EDITOR false
#define LUA_SUPPORT true

#include "../ecs/world.h"
#include "commanche2d.h"
#include "glm/glm.hpp"
#include <iostream>
#include <sol/sol.hpp>

#if EDITOR
#include "../editor/editor.h"
#endif

#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1920

const int FPS = 60;
const int FRAME_TIME_LENGTH = (1000 / FPS);


class Game {
    public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void Setup();
    void LoadLevel(int number);
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();

    bool isRunning;
    static glm::vec2 cursor;
    static int mapHeight;
    static int mapWidth;
    std::shared_ptr<World> world;

    CommancheCamera camera;
    CommancheRenderer* renderer;
    private:
    bool isDebug = false;
    int windowWidth;
    int windowHeight;
    int tickLastFrame;
    sol::state lua;
    Map* map;
    EventBus* bus;

#if EDITOR
    Editor* editor;
#endif
};
#endif
