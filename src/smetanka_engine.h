#pragma once

#include <stdint.h>
#include "ext/raylib.h"
#include "smetanka_math.h"

typedef Vector2 Vector2;
typedef Rectangle Rectangle;
typedef Sound Sound;
typedef Music Music;
typedef Font Font;
typedef Image Image;
typedef  Texture Texture;
typedef Color Color;

typedef struct {
    const char *windowName;
    int windowWidth;
    int windowHeight;
    int virtualWidth;
    int virutalHeight;
    unsigned int configFlags;
    int fps;
    const char *resourceDir;
    uint8_t fullscreen;
} SmeInitData;

void smeInit(SmeInitData *data);
bool smeShouldClose();
void smeClose();

RenderTexture SmeLoadRenderTexture(int width, int height);

void smeBeginTextureMode(RenderTexture texture);
void smeEndTextureMode();

void smeRenderVirtualScreen(RenderTexture *virtualScreen, Rectangle *sourceRec, Rectangle *destRec);

Shader smeLoadShader(const char *filename);

float smeGetFrameTime();

void smeDrawTextureRec(Texture texture, Rectangle source, Vector2 position, Color tint);
void smeDrawText(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);
void smeDrawTextF(
    Font font,
    Vector2 position,
    float fontSize,
    float spacing,
    Color tint,
    const char *format,
    ...
);

Font smeloadFont(const char *path);

Image smeLoadImage(const char *filename);
void smeUnloadImage(Image image);

Color *smeLoadImageColors(Image image);
void smeUnloadImageColors(Color *colors);

Texture smeLoadTexture(const char *filename);
Texture smeLoadPixelTexture(const char *filename);
void smeUnloadTexture(Texture2D texture);

Sound smeLoadSound(const char *filename);
void smeUnloadSound(Sound sound);

Music smeLoadMusicStream(const char *filename);
void smeUnloadMusicStream(Music music);

void smePlayMusicStream(Music music);
void smeStopMusicStream(Music music);

unsigned char *smeLoadFileData(const char *filename, int *dataSize);
void smeUnloadFileData(unsigned char *data);
bool smeSaveFileData(const char *filename, void *data, int dataSize);
