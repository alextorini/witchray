#pragma once

#include <stdint.h>
#include "smetanka_engine.h"

typedef struct {
    Vector2 move;
    uint8_t fullscreen_toggle;
    uint8_t any_key;
} InputState;

InputState input_read();
