#pragma once

#include <math.h>
#include <stdint.h>
#include "ext/raymath.h"

typedef Vector2 Vector2;

static inline uint8_t aabb_intersect(
    float ax, float ay, float aw, float ah,
    float bx, float by, float bw, float bh)
{
    return ax < bx + bw &&
           ax + aw > bx &&
           ay < by + bh &&
           ay + ah > by;
}

static inline float xy_magnitude(float x, float y) {
    return sqrt(x * x + y * y);
}

static inline Vector2 vector2_sum(Vector2 a, Vector2 b) {
    return (Vector2){
        .x = a.x + b.x,
        .y = a.y + b.y
    };
}

static inline Vector2 vector2_scale(Vector2 v, float f) {
    return (Vector2){
        .x = v.x * f,
        .y = v.y * f
    };
}

