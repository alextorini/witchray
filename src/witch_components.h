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
    uint16_t startFrame;
    uint16_t endFrame;
    float frameTime;
    uint8_t loop;
} AnimationClip;

typedef struct {
    AnimationClip *clips;
    uint32_t count;
} AnimationSet;

typedef struct {
    AnimationSet *set;
    uint16_t currentClip;
    uint16_t currentFrame;
    float timer;
} Animation;

typedef struct {
    Position offset;
    Color color;
    Font font;
    float fontSize;
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

void initComponents(EcsComponentId (*cmpIds)[CMP_COUNT]);
