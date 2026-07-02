#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "smetanka_ecs.h"
#include "witch_collisions.h"
#include "witch_components.h"
#include "witch_resources.h"

#define GAME_VERSION 0

#define VIRTUAL_WIDTH 640
#define VIRTUAL_HEIGHT 360

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define STATE_START_SCREEN 0
#define STATE_GAME_INIT 1
#define STATE_GAMEPLAY 2
#define STATE_PAUSE 3
#define STATE_GAMEOVER_INIT 4
#define STATE_GAMEOVER 5

#define SKY_COLOR CLITERAL(Color){255, 128, 156, 255}

#define BG_LAYER_1_SPEED 50.0f
#define BG_LAYER_2_SPEED 150.0f

#define PLAYER_START_POS {50, 50}
#define PLAYER_DEFAULT_FRAME {0.0f, 0.0f, 41.0f, 27.0f}
#define PLAYER_SPEED 3.0f
#define GAMEPAD_DEADZONE 0.1f
#define PLAYER_IDLE_ANIM_SPEED 0.25f

#define WR_MALLOC malloc
#define WR_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define WR_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define WR_CALLOC calloc
#define WR_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define WR_FREE free

typedef struct {
    FrameCollisionMap collisions;
} Enemies;

typedef struct {
    FrameCollisionMap collisions;
} Fireballs;

typedef struct {
    EcsEntityHandle handle;
    FrameCollisionMap collisions;
} Player;

typedef struct {
    EcsEntityHandle player_handle;
    Player player;
    Enemies enemies;
    Fireballs fireballs;
    EcsEntityHandle backgrounds[2][2];
    ResourceMap resources;
    EcsComponentId components[CMP_COUNT];
    int state;
    float seconds_alive;
    uint64_t enemies_killed;
    uint64_t score;
    uint64_t highscore;
} Game;

extern Game game;

extern uint8_t should_close;

