#pragma once

#include "witch_components.h"

typedef enum {
    ANIMATION_IDLE,
    ANIMATION_DYING,
    ANIMATION_COUNT
} AnimationIndex;

void switchAnimation(Animation *animation, AnimationIndex newState);
