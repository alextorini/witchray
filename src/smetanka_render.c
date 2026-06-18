#include "smetanka_render.h"

Texture load_pixel_texture(const char *path) {
    Texture texture = LoadTexture(path);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    return texture;
}
