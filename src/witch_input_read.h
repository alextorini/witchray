#pragma once

#include <stdint.h>
#include "smetanka_engine.h"

typedef struct {
    Vector2 move;
    uint8_t fullscreenToggle;
    uint8_t anyKey;
} InputState;

InputState inputRead();
