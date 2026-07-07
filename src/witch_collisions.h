#pragma once

#include <stdint.h>
#include "smetanka_engine.h"

typedef struct {
    uint8_t *frame_mask_list;
    uint32_t width;
    uint32_t height;
    uint32_t count;
} FrameCollisionMap;

void init_collision_map(const char *image_path, Rectangle frame, uint32_t frame_count, FrameCollisionMap *collision_map);

uint8_t check_collision(
    const FrameCollisionMap *a_map, uint32_t a_frame_index, float ax, float ay,
    const FrameCollisionMap *b_map, uint32_t b_frame_index, float bx, float by
);
