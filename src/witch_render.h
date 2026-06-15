#pragma once

#include "ext/raylib.h"

#include "smetanka_ecs.h"

Texture load_pixel_texture(const char *path);

EcsEntityId *add_parallax_background_layer(Texture *texture, float speed);
