#include "box2d/box2d.h"
#include "game/game.h"
#include <sol/sol.hpp>
#include "eventmgr/EventBus.h"
#include "flecs.h"

struct Example {
  double x, y;
};

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

