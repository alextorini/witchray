#include "smetanka_engine.h"
#include "ext/resource_dir.h"
#include "ext/raylib.h"
#include <stdio.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

static EM_BOOL OnCanvasResize(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
    double w, h;
    emscripten_get_element_css_size("canvas", &w, &h);
    SetWindowSize((int)w, (int)h);
    return EM_TRUE;
}

static EM_BOOL OnFullscreenChange(int eventType, const EmscriptenFullscreenChangeEvent *fsEvent, void *userData) {
    double w, h;
    emscripten_get_element_css_size("canvas", &w, &h);
    SetWindowSize((int)w, (int)h);
    return EM_TRUE;
}
#endif

typedef struct {
    RenderTexture virtualScreen;
    Rectangle sourceRec;
    Rectangle destRec;
} Screen;

typedef RenderTexture RenderTexture;

void smeInit(SmeInitData *initData) {
    SetConfigFlags(initData->configFlags);

    SetTargetFPS(initData->fps);

    InitWindow(initData->windowWidth, initData->windowHeight, initData->windowName);

#if defined(PLATFORM_WEB)
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_TRUE, OnCanvasResize);
    emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, NULL, EM_TRUE, OnFullscreenChange);
#endif

    InitAudioDevice();

    SearchAndSetResourceDir(initData->resourceDir);
}

bool smeShouldClose() {
    return WindowShouldClose();
}

void smeClose() {
    CloseAudioDevice();
    CloseWindow();
}

RenderTexture smeLoadRenderTexture(int width, int height) {
   return LoadRenderTexture(width, height);
}

void smeBeginTextureMode(RenderTexture texture) {
    BeginTextureMode(texture);
}

void smeEndTextureMode() {
    EndTextureMode();
}

void smeRenderVirtualScreen(RenderTexture *virtualScreen, Rectangle *sourceRec, Rectangle *destRec) {
    if (IsKeyPressed(KEY_F)) {
        ToggleFullscreen();
    }

    destRec->width = (float)GetScreenWidth();
    destRec->height = (float)GetScreenHeight();

    DrawTexturePro(virtualScreen->texture, *sourceRec, *destRec,
                   CLITERAL(Vector2){0, 0}, 0.0f, WHITE);

    EndDrawing();
}

Shader smeLoadShader(const char *filename) {
    return LoadShader(0, filename);
}

float smeGetFrameTime() {
    return GetFrameTime();
}

void smeDrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint) {
    DrawTextureRec(texture, source, position, tint);
}

void smeDrawText(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint) {
    DrawTextEx(font, text, position, fontSize, spacing, tint);
}

void smeDrawTextF(
    Font font,
    Vector2 position,
    float fontSize,
    float spacing,
    Color tint,
    const char *format,
    ...
) {
    char buffer[256];

    va_list args;
    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    smeDrawText(font, buffer, position, fontSize, spacing, tint);
}

Font smeloadFont(const char *path) {
    int codepoints[512] = {0};
    for (int i = 0; i < 95; i++)
        codepoints[i] = 32 + i;
    for (int i = 0; i < 255; i++)
        codepoints[96 + i] = 0x400 + i;

    return LoadFontEx(path, 18, codepoints, 350);
}

Image smeLoadImage(const char *filename) {
    return LoadImage(filename);
}

void smeUnloadImage(Image image) {
    return UnloadImage(image);
}

Color *smeLoadImageColors(Image image) {
    return LoadImageColors(image);
}

void smeUnloadImageColors(Color *colors) {
    return UnloadImageColors(colors);
}

Texture smeLoadTexture(const char *filename) {
    return LoadTexture(filename);
}

Texture smeLoadPixelTexture(const char *filename) {
    Texture texture = smeLoadTexture(filename);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    return texture;
}

void smeUnloadTexture(Texture2D texture) {
    UnloadTexture(texture);
}

Sound smeLoadSound(const char *filename) {
    return LoadSound(filename);
}

void smeUnloadSound(Sound sound) {
    UnloadSound(sound);
}

void smePlaySound(Sound sound) {
    PlaySound(sound);
}

void smeSetSoundVolume(Sound sound, float volume) {
    SetSoundVolume(sound, volume);
}

Music smeLoadMusicStream(const char *filename) {
    return LoadMusicStream(filename);
}

void smeUnloadMusicStream(Music music) {
    UnloadMusicStream(music);
}

void smePlayMusicStream(Music music) {
    PlayMusicStream(music);
}

void smeStopMusicStream(Music music) {
    StopMusicStream(music);
}

unsigned char *smeLoadFileData(const char *filename, int *dataSize) {
    return LoadFileData(filename, dataSize);
}

void smeUnloadFileData(unsigned char *data) {
    UnloadFileData(data);
}

bool smeSaveFileData(const char *filename, void *data, int dataSize) {
    return SaveFileData(filename, data, dataSize);
}
