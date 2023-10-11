#pragma once
#include "glm/glm.hpp"
#include <iostream> 
#include <sol/sol.hpp>

const int FPS = 60;
const int FRAME_TIME_LENGTH = (1000 / FPS);


class Game {
    public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void Setup();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();

    bool isRunning;

    private:
    int tickLastFrame;
    sol::state lua;
};
