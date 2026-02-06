#pragma once

#include "raylib.h"
#include "secs.h"

Texture load_pixel_texture(const char *path);

void add_parallax_background_layer(EcsSpace *space, Texture *texture, float speed);
