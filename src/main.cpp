#include "box2d/box2d.h"
#include "game/game.h"
#include <sol/sol.hpp>
#include "eventmgr/EventBus.h"

int main(int argc, char* argv[]) {
    Game game;
    game.Initialize();
    game.Setup();

    while (game.isRunning) {
        game.ProcessInput();
        CommancheRenderer::Instance->BeginDraw();
        game.Update();
        CommancheRenderer::Instance->EndDraw();
        CommancheRenderer::Instance->RenderStart();
        game.Render();
        CommancheRenderer::Instance->RenderEnd();
    }
    return 0;
}

