#pragma once

#include "ext/raylib.h"

#include "smetanka_ecs.h"

typedef Vector2 Position;
typedef Vector2 Velocity;
typedef bool IsParallax;

typedef struct {
    Texture *spritesheet;
    Rectangle frame;
} Render;

typedef struct {
    uint16_t start_frame;
    uint16_t end_frame;
    float frame_time;
    bool loop;
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
    Texture layer_1;
    Texture layer_2;
} Background;

typedef struct {
    EcsComponent *pos;
    EcsComponent *vel;
    EcsComponent *rndr;
    EcsComponent *anim;
    EcsComponent *prlx;
} EcsComponentPointers;

typedef struct {
    EcsEntity *plr;
    EcsEntity *background_layers[2][2];
    EcsEntity **enemies;
} EcsEntityPointers;

typedef struct {
    EcsComponentPointers cmpnts;
    EcsEntityPointers entts;
} EcsPointers;

extern EcsPointers ecs_ptrs;
extern EcsSpace *spc;

void init();

void update_and_draw();

void unload();
