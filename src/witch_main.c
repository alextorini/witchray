#include "ext/resource_dir.h"

#include "witch_core.h"
#include "smetanka_misc.h"

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    SetTargetFPS(60);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "WitchRay");

    InitAudioDevice();

    SearchAndSetResourceDir("res");

    init();

    RenderTexture2D virtualScreen = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    Rectangle sourceRec = {0.0f, 0.0f, (float)virtualScreen.texture.width, (float)-virtualScreen.texture.height};
    Rectangle destRec = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};

    while (!WindowShouldClose() && !should_close) {
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
