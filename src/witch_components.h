#pragma once

#include <stdint.h>
#include "smetanka_ecs.h"
#include "smetanka_engine.h"

typedef Vector2 Position;
typedef Vector2 Velocity;
typedef uint8_t IsParallax;
typedef uint8_t IsEnemy;

typedef struct {
    int32_t damage;
} Fireball;

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

typedef struct {
    Position offset;
    Color color;
    Font font;
    float font_size;
    float spacing;
    char text[128];
} TextRender;

typedef enum {
    CMP_POSITION,
    CMP_VELOCITY,
    CMP_RENDER,
    CMP_ANIMATION,
    CMP_PARALLAX,
    CMP_ENEMY,
    CMP_TEXT_RENDER,
    CMP_FIREBALL,
    CMP_COUNT
} ComponentIndex;

void init_components(EcsComponentId (*cmp_ids)[CMP_COUNT]);
