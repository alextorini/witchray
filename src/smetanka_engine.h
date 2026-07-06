#pragma once

#include <stdint.h>
#include "ext/raylib.h"
#include "ext/raymath.h"

typedef Vector2 Vector2;
typedef Rectangle Rectangle;

typedef struct {
    const char *window_name;
    int window_width;
    int window_height;
    int virtual_width;
    int virutal_height;
    unsigned int config_flags;
    int fps;
    const char *resource_dir;
    uint8_t fullscreen;
} SmetankaInitData;

void smetanka_init(SmetankaInitData *data);

void smetanka_begin_texture_mode();

void smetanka_end_texture_mode();

void smetanka_render();
