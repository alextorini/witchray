#pragma once

#include <stdint.h>
#include "ext/raylib.h"
#include "smetanka_ecs.h"

typedef Vector2 Position;
typedef Vector2 Velocity;
typedef uint8_t IsParallax;
typedef uint8_t IsEnemy;

typedef struct {
    Texture *spritesheet;
    Rectangle frame;
    // int priority;
} Render;

typedef struct {
    uint16_t start_frame;
    uint16_t end_frame;
    float frame_time;
    uint8_t loop;
} AnimationClip;

typedef struct {
    AnimationClip *clips;
    uint32_t count;
} AnimationSet;

typedef struct {
    AnimationSet *set;
    uint16_t current_clip;
    uint16_t current_frame;
    float timer;
} Animation;

typedef enum {
    CMP_POSITION,
    CMP_VELOCITY,
    CMP_RENDER,
    CMP_ANIMATION,
    CMP_PARALLAX,
    CMP_ENEMY,
    CMP_COUNT
} ComponentIndex;

void init_components(EcsComponentId (*cmp_ids)[CMP_COUNT]);
