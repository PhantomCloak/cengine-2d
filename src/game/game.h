#ifndef GAME_H
#define GAME_H

#define LUA_SUPPORT true

#include "../ecs/world.h"
#include "commanche2d.h"
#include "glm/glm.hpp"
#include <iostream> 
#include <sol/sol.hpp>
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1920

const int FPS = 500;
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

    private:
    bool isDebug = false;
    int windowWidth;
    int windowHeight;
    int tickLastFrame;
    sol::state lua;
    EventBus* bus;

};
#endif
