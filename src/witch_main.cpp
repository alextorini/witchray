#include "smetanka_engine.h"
#include "witch_core.h"
#include "witch_game.h"

int main() {
    SmeInitData initData = {
        .windowName = "Retro Witch",
        .windowWidth = WINDOW_WIDTH,
        .windowHeight = WINDOW_HEIGHT,
        .configFlags = FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI,
        .fps = 60,
        .resourceDir = "res",
        .fullscreen = 0
    };
    smeInit(&initData);

    Game game;

    init(&game);

    RenderTexture virtualScreen = smeLoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    Rectangle virtualScreenRec = (Rectangle)
        {0.0f, 0.0f, (float)virtualScreen.texture.width, (float)-virtualScreen.texture.height};
    Rectangle windowRec = (Rectangle){0.0f, 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT};

    while (!smeShouldClose() && !game.shouldClose) {
        smeBeginTextureMode(virtualScreen);

        updateAndRender(&game, &virtualScreen, smeGetFrameTime());

        smeRenderVirtualScreen(&virtualScreen, &virtualScreenRec, &windowRec);
    }

    unload(&game);

    return 0;
}
