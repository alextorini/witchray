#pragma once

#include "secs.h"
#include "raylib.h"

typedef Vector2 Position;
typedef Vector2 Velocity;
typedef bool IsParalax;

typedef struct {
    Texture *spritesheet;
    Rectangle frame;
} Render;

typedef struct {
    Texture layer_1;
    Texture layer_2;
} Background;

typedef struct {
    EcsComponent *position;
    EcsComponent *velocity;
    EcsComponent *render;
    EcsComponent *paralax;
} EcsComponentPointers;

typedef struct {
    EcsEntity *player;
    EcsEntity *background_layers[2][2];
    EcsEntity **enemies;
} EcsEntityPointers;

typedef struct {
    EcsComponentPointers components;
    EcsEntityPointers entities;
} EcsPointers;

extern EcsPointers ecs_pointers;

void init();

void update_and_draw();

void unload();
