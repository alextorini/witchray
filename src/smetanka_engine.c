#include "smetanka_engine.h"
#include "ext/resource_dir.h"
#include "ext/raylib.h"

typedef struct {
    RenderTexture2D virtual_screen;
    Rectangle source_rec;
    Rectangle dest_rec;
} Screen;

static Screen screen;

void smetanka_init(SmetankaInitData *init_data) {
    SetConfigFlags(init_data->config_flags);

    SetTargetFPS(init_data->fps);

    InitWindow(init_data->window_width, init_data->window_height, init_data->window_name);

    InitAudioDevice();

    SearchAndSetResourceDir(init_data->resource_dir);

    screen.virtual_screen = LoadRenderTexture(init_data->virtual_width, init_data->virutal_height);
    screen.source_rec = (Rectangle)
        {0.0f, 0.0f, (float)screen.virtual_screen.texture.width, (float)-screen.virtual_screen.texture.height};
    screen.dest_rec = (Rectangle)
        {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
}

void smetanka_begin_texture_mode() {
    BeginTextureMode(screen.virtual_screen);
}

void smetanka_end_texture_mode() {
    EndTextureMode();
}

void smetanka_render() {
    BeginDrawing();

    if (IsKeyPressed(KEY_F)) {
        ToggleFullscreen();
        screen.dest_rec.width = (float)GetScreenWidth();
        screen.dest_rec.height = (float)GetScreenHeight();
    }

    DrawTexturePro(screen.virtual_screen.texture, screen.source_rec, screen.dest_rec,
                   CLITERAL(Vector2){0, 0}, 0.0f, WHITE);

    EndDrawing();
}
