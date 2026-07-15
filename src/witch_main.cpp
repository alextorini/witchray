#include "smetanka_engine.h"
#include "witch_core.h"

int main() {
    SmeInitData initData = {
        .windowName = "Witch Alice",
        .windowWidth = WINDOW_WIDTH,
        .windowHeight = WINDOW_HEIGHT,
        .virtualWidth = VIRTUAL_WIDTH,
        .virutalHeight = VIRTUAL_HEIGHT,
        .configFlags = FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI,
        .fps = 60,
        .resourceDir = "res",
        .fullscreen = 0
    };
    smeInit(&initData);

    Game *game = WR_MALLOC_TYPE(Game);

    init(game);

    while (!smeShouldClose() && !shouldClose) {
        smeBeginTextureMode();

        updateAndDraw(game, smeGetFrameTime());

        smeEndTextureMode();

        smeRender();
    }

    unload(game);

    return 0;
}
