#include "smetanka_engine.h"
#include "ext/resource_dir.h"
#include "ext/raylib.h"
#include <stdio.h>

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

bool window_should_close() {
    return WindowShouldClose();
}

void smetanka_close() {
    CloseAudioDevice();
    CloseWindow();
}

void begin_texture_mode() {
    BeginTextureMode(screen.virtual_screen);
}

void end_texture_mode() {
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

float get_dt() {
    return GetFrameTime();
}

void draw_texture_rec(Texture2D texture, Rectangle source, Vector2 position, Color tint) {
    DrawTextureRec(texture, source, position, tint);
}

void draw_text(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint) {
    DrawTextEx(font, text, position, fontSize, spacing, tint);
}

void draw_text_format(
    Font font,
    Vector2 position,
    float font_size,
    float spacing,
    Color tint,
    const char *format,
    ...
)
{
    char buffer[256];

    va_list args;
    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    draw_text(font, buffer, position, font_size, spacing, tint);
}

Font load_font(const char *path) {
    int codepoints[512] = {0};
    for (int i = 0; i < 95; i++)
        codepoints[i] = 32 + i;
    for (int i = 0; i < 255; i++)
        codepoints[96 + i] = 0x400 + i;

    return LoadFontEx(path, 18, codepoints, 350);
}

Image load_image(const char *filename) {
    return LoadImage(filename);
}

void unload_image(Image image) {
    return UnloadImage(image);
}

Color *load_image_colors(Image image) {
    return LoadImageColors(image);
}

void unload_image_colors(Color *colors) {
    return UnloadImageColors(colors);
}

Texture load_texture(const char *filename) {
    return LoadTexture(filename);
}

Texture load_pixel_texture(const char *filename) {
    Texture texture = load_texture(filename);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    return texture;
}

void unload_texture(Texture2D texture) {
    UnloadTexture(texture);
}

Sound load_sound(const char *filename) {
    return LoadSound(filename);
}

void unload_sound(Sound sound) {
    UnloadSound(sound);
}

Music load_music_stream(const char *filename) {
    return LoadMusicStream(filename);
}

void unload_music_stream(Music music) {
    UnloadMusicStream(music);
}

void play_music_stream(Music music) {
    PlayMusicStream(music);
}

void stop_music_stream(Music music) {
    StopMusicStream(music);
}

unsigned char *load_file_data(const char *filename, int *data_size) {
    return LoadFileData(filename, data_size);
}

void unload_file_data(unsigned char *data) {
    UnloadFileData(data);
}

bool save_file_data(const char *filename, void *data, int data_size) {
    return SaveFileData(filename, data, data_size);
}
