#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "smetanka_ecs.h"
#include "witch_collisions.h"
#include "witch_resources.h"
#include "witch_color.h"

#define GAME_VERSION 0

constexpr int VIRTUAL_WIDTH = 640;
#define VIRTUAL_HEIGHT 360

#define PLAYSCREEN_WIDTH 640
#define PLAYSCREEN_HEIGHT 360

#define UI_WIDTH 640
#define UI_HEIGHT 40

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define STATE_START_SCREEN 0
#define STATE_GAME_INIT 1
#define STATE_GAMEPLAY 2
#define STATE_PAUSE 3
#define STATE_GAMEOVER_INIT 4
#define STATE_GAMEOVER 5


#define BG_LAYER_1_SPEED 25.0f
#define BG_LAYER_2_SPEED 75.0f

#define PICKAPABLE_SPEED 50.0f

#define PLAYER_START_POS {50, 50}
#define PLAYER_DEFAULT_FRAME {0.0f, 0.0f, 41.0f, 27.0f}
#define PLAYER_SPEED 3.0f
#define PLAYER_IDLE_ANIM_SPEED 0.25f

#define PLAYER_MAX_HEALTH 10.0f

#define PLAYER_FIREBALL_COOLDOWN 0.5f
#define PLAYER_ICEBALL_COOLDOWN 1.0f
#define PLAYER_STARBALL_COOLDOWN 2.0f

#define PLAYER_WEAPON_FIREBALL 1
#define PLAYER_WEAPON_ICEBALL 2
#define PLAYER_WEAPON_STARBALL 3

#define PLAYER_DEATH_COOLDOWN 1.0f

#define SPAWN_COOLDOWN 1.0f

#define WR_MALLOC malloc
#define WR_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define WR_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define WR_CALLOC calloc
#define WR_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define WR_FREE free

typedef struct {
    FrameCollisionMap collisions;
    float deathCooldown;
} EnemyRegistry;

typedef struct {
    struct {
        FrameCollisionMap fireball;
        FrameCollisionMap iceball;
        FrameCollisionMap starball;
    } collisions;
    float deathCooldown;
} SpellRegistry;

typedef struct {
    EcsEntityHandle handle;
    FrameCollisionMap collisions;
    uint64_t level;
    float exp;
    float max_exp;
    float deathCooldown;
} Player;


typedef struct {
    float speed;
    FrameCollisionMap collisions;
} PickapableRegistry;

typedef struct {
    Player player;
    EnemyRegistry enemies;
    SpellRegistry spells;
    PickapableRegistry pickups;
    EcsEntityHandle backgrounds[2][2];
    ResourceRegistry resources;
    ColorIndex skyColor;
    uint64_t enemiesKilled;
    uint64_t score;
    uint64_t highscore;
    int state;
    int requestedState;
    float timer;
    float enemySpawnCooldown;
    float winCooldown;
    uint8_t win;
    uint8_t spawnEnemies;
    uint8_t castSpells;
    uint8_t pause;
    uint8_t shouldClose;
} Game;

extern uint8_t shouldClose;

static inline void wrMalloc(size_t size) {
    malloc(size);
}
