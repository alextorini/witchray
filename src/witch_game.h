#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "smetanka_ecs.h"
#include "witch_collisions.h"
#include "witch_components.h"

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

#define SKY_COLOR_1 CLITERAL(Color){99, 155, 255, 255}
#define SKY_COLOR_2 CLITERAL(Color){255, 128, 156, 255}

#define BG_LAYER_1_SPEED 25.0f
#define BG_LAYER_2_SPEED 75.0f

#define PLAYER_START_POS {50, 50}
#define PLAYER_DEFAULT_FRAME {0.0f, 0.0f, 41.0f, 27.0f}
#define PLAYER_SPEED 3.0f
#define PLAYER_IDLE_ANIM_SPEED 0.25f

#define PLAYER_MAX_HEALTH 10.0f

#define WR_MALLOC malloc
#define WR_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define WR_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define WR_CALLOC calloc
#define WR_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define WR_FREE free

typedef struct {
    FrameCollisionMap collisions;
    float deathCooldown;
} Enemies;

typedef struct {
    FrameCollisionMap collisions;
    float deathCooldown;
} Fireballs;

typedef struct {
    EcsEntityHandle handle;
    FrameCollisionMap collisions;
    float deathCooldown;
} Player;

typedef enum {
    SOUND_SHOOT,
    SOUND_EXPLOSION,
    SOUND_PLAYER_DEATH,
    SOUND_PAUSE,
    SOUND_COUNT
} SoundIndex;

typedef enum {
    MUSIC_MAIN,
    MUSIC_COUNT
} MusicIndex;

typedef enum {
    SPRITESHEET_PLAYER,
    SPRITESHEET_ENEMY,
    BACKGROUND_URBAN,
    BACKGROUND_CLOUDS,
    SPRITESHEET_FIREBALL,
    SPRITE_COUNT
} SpriteIndex;

typedef enum {
    FONT_MAIN,
    FONT_COUNT
} FontIndex;

typedef struct {
    struct {
        Shader shader;
        int colorLoc;
        int strengthLoc;
    } damageFlash;
} ShadersMap;


typedef struct {
    Font fonts[FONT_COUNT];
    Texture sprites[SPRITE_COUNT];
    Sound sounds[SOUND_COUNT];
    Music music[MUSIC_COUNT];
    ShadersMap shaders;
    int flashColorLoc;
    int flashStrengthLoc;
} ResourceMap;

typedef struct {
    EcsEntityHandle player_handle;
    Player player;
    Enemies enemies;
    Fireballs fireballs;
    EcsEntityHandle backgrounds[2][2];
    ResourceMap resources;
    EcsComponentId components[CMP_COUNT];
    int state;
    int requestedState;
    float timer;
    float enemySpawnCooldown;
    Color skyColor;
    uint64_t enemiesKilled;
    uint64_t score;
    uint64_t highscore;
    uint8_t pause;
    uint8_t shouldClose;
} Game;

extern uint8_t shouldClose;

static inline void wrMalloc(size_t size) {
    malloc(size);
}
