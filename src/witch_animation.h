#pragma once

#include "witch_components.h"

typedef enum {
    ANIMATION_IDLE = 0,
    ANIMATION_DYING = 1,
    ANIMATION_COUNT = 2
} AnimationIndex;

void switchAnimation(Animation *animation, AnimationIndex newState);
