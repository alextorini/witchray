#include "smetanka_render.h"

Texture load_pixel_texture(const char *path) {
    Texture texture = LoadTexture(path);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    return texture;
}

Font load_font(const char *path) {
    int codepoints[512] = {0};
    for (int i = 0; i < 95; i++)
        codepoints[i] = 32 + i;
    for (int i = 0; i < 255; i++)
        codepoints[96 + i] = 0x400 + i;

    return LoadFontEx(path, 18, codepoints, 350);
}
