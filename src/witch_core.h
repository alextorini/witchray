#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "ext/raylib.h"

#include "smetanka_ecs.h"
#include "witch_resources.h"
#include "witch_components.h"
#include "witch_player.h"

#define VIRTUAL_WIDTH 640
#define VIRTUAL_HEIGHT 360

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define SKY_COLOR CLITERAL(Color){255, 128, 156, 255}

#define BG_LAYER_1_SPEED 50.0f
#define BG_LAYER_2_SPEED 150.0f

#define PLAYER_START_POS {50, 50}
#define PLAYER_DEFAULT_FRAME {0.0f, 0.0f, 41.0f, 27.0f}
#define PLAYER_SPEED 3.0f
#define GAMEPAD_DEADZONE 0.1f
#define PLAYER_IDLE_ANIM_SPEED 0.25f

#define ENEMY_DEFAULT_FRAME {0.0f, 0.0f, 32.0f, 32.0f}

#define WR_MALLOC malloc
#define WR_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define WR_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define WR_CALLOC calloc
#define WR_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define WR_FREE free

typedef struct {
    EcsEntityHandle player_handle;
    EcsEntityHandle backgrounds[2][2];
    ResourceMap resources;
    EcsComponentId components[CMP_COUNT];
} Game;

extern Game game;

extern uint8_t should_close;

void init();

void update_and_draw();

void unload();
