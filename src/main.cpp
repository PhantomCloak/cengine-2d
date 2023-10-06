#include "game/game.h"

int main(int argc, char* argv[]) {
    Game game;
    game.Initialize();
    game.Setup();

    while (game.isRunning) {
        //game.ProcessInput();
#if EDITOR
        CommancheRenderer::Instance->BeginDraw();
        game.Update();
        CommancheRenderer::Instance->EndDraw();
        CommancheRenderer::Instance->RenderStart();
        game.Render();
        CommancheRenderer::Instance->RenderEnd();
#else
        CommancheRenderer::Instance->RenderStart();
        game.Update();
        game.Render();
        CommancheRenderer::Instance->RenderEnd();
#endif
    }
    return 0;
}

