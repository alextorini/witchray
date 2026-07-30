#pragma once

#include "smetanka_engine.h"

typedef enum {
    SKY_COLOR_1 = 0,
    SKY_COLOR_2 = 1,
    COLOR_COUNT
} ColorIndex;

constexpr Color kColors[COLOR_COUNT] = {
    {99, 155, 255, 255}, /* SKY_COLOR_1 */
    {255, 128, 156, 255} /* SKY_COLOR_2 */
};
