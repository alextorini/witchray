#pragma once

#include "ext/raylib.h"

#include "smetanka_ecs.h"

#define VIRTUAL_WIDTH 640
#define VIRTUAL_HEIGHT 360

#define SKY_COLOR CLITERAL(Color){255, 128, 156, 255}

#define BG_LAYER_1_SPEED 50.0f
#define BG_LAYER_2_SPEED 150.0f

#define PLAYER_SPEED 3.0f

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
    EcsComponentId pos;
    EcsComponentId vel;
    EcsComponentId rndr;
    EcsComponentId anim;
    EcsComponentId prlx;
} EcsComponentHandles;

typedef struct {
    EcsEntityId plr;
    EcsEntityId background_layers[2][2];
    EcsEntityId *enemies;
} EcsEntityHandles;

typedef struct {
    EcsComponentHandles cmpnts;
    EcsEntityHandles entts;
} EcsHandles;

typedef struct {
    Texture player;
    Texture enemy;
} Spritesheets;

extern Spritesheets sprtshts;

extern EcsHandles ecs_hndls;

void init();

void update_and_draw();

void unload();
