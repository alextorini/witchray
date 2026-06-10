#include "ext/resource_dir.h"

#include "witch_core.h"
#include "smetanka_misc.h"

const int VIRTUAL_WIDTH = 640;
const int VIRTUAL_HEIGHT = 360;

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    SetTargetFPS(60);
    InitWindow(1280, 720, "WitchRay");
    InitAudioDevice();
    SearchAndSetResourceDir("resources");

    init();

    RenderTexture2D virtualScreen = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    Rectangle sourceRec = {0.0f, 0.0f, (float)virtualScreen.texture.width, (float)-virtualScreen.texture.height};
    Rectangle destRec = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};

    while (!WindowShouldClose()) {
        BeginTextureMode(virtualScreen);

        update_and_draw();

        EndTextureMode();

        BeginDrawing();
        DrawTexturePro(virtualScreen.texture, sourceRec, destRec, CLITERAL(Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    unload();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
