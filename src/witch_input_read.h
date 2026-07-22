#pragma once

#include <stdint.h>
#include "smetanka_engine.h"

typedef struct {
    Vector2 move;
    uint8_t fullscreenToggle;
    uint8_t pause;
    uint8_t activate;
} InputState;

InputState inputRead();
