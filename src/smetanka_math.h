#pragma once

#include <math.h>
#include <stdint.h>
#include "ext/raymath.h"

#define PI 3.14159265358979323846f

typedef Vector2 Vector2;

static inline uint8_t aabbIntersect(
    float ax, float ay, float aw, float ah,
    float bx, float by, float bw, float bh)
{
    return ax < bx + bw &&
           ax + aw > bx &&
           ay < by + bh &&
           ay + ah > by;
}

static inline float xyMagnitude(float x, float y) {
    return sqrt(x * x + y * y);
}

static inline Vector2 xyNormalize(float x, float y) {
    float magnitude = xyMagnitude(x, y);

    if (magnitude == 0.0f) {
        return Vector2{0.0f, 0.0f};
    }

    return Vector2{x / magnitude, y / magnitude};
}

static inline Vector2 vector2Sum(float x1, float y1, float x2, float y2) {
    return (Vector2){
        .x = x1 + x1,
        .y = y1 + y2
    };
}

static inline Vector2 vector2Sum(Vector2 a, Vector2 b) {
    return (Vector2){
        .x = a.x + b.x,
        .y = a.y + b.y
    };
}

static inline Vector2 vector2Scale(Vector2 v, float f) {
    return (Vector2){
        .x = v.x * f,
        .y = v.y * f
    };
}

