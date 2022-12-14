#include "game/game.h"
#include <sol/sol.hpp>

void TestLua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script_file("./assets/scripts/test.lua");
    bool isFs = lua["config"]["fullscreen"];

    std::cout << "THE VALUE is " << isFs << std::endl;
}

int main(int argc, char* argv[]) {
    Game game;
    game.Initialize();
    game.Setup();

    while (game.isRunning) {
        game.ProcessInput();
        game.Update();
        game.Render();
    }
    return 0;
}
