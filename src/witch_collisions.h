#pragma once

#include <stdint.h>
#include "smetanka_engine.h"

typedef struct {
    uint8_t *frameMaskList;
    uint32_t width;
    uint32_t height;
    uint32_t count;
} FrameCollisionMap;

void initCollisionMap(const char *imagePath, Rectangle frame, uint32_t frameCount, FrameCollisionMap *collisionMap);

uint8_t checkCollision(
    const FrameCollisionMap *aMap, uint32_t aFrameIndex, float ax, float ay,
    const FrameCollisionMap *bMap, uint32_t bFrameIndex, float bx, float by
);
