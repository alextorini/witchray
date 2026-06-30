#pragma once

#include "ext/raylib.h"
#include "stdint.h"

typedef struct {
    uint8_t *mask_list;
    uint32_t width;
    uint32_t height;
    uint32_t count;
} FrameCollisionMap;

void init_collision_map(const char *image_path, Rectangle frame, uint32_t frame_count, FrameCollisionMap collision_map);
