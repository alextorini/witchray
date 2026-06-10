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
    Texture layer_1;
    Texture layer_2;
} Background;

typedef struct {
    EcsComponent *pos;
    EcsComponent *vel;
    EcsComponent *rndr;
    EcsComponent *is_prlx;
    // EcsComponent *is_plr;
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
