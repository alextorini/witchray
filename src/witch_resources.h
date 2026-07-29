#pragma once

#include "smetanka_engine.h"

#define ENEMY_IMAGE_PATH "images/enemy.png"
#define FIREBALL_IMAGE_PATH "images/outlined_fireballs.png"
#define ICEBALL_IMAGE_PATH "images/iceball.png"
#define STARBALL_IMAGE_PATH "images/starball.png"
#define PLAYER_IMAGE_PATH "images/player.png"
#define EXP_CRYSTAL_PATH "images/exp_crystal.png"

typedef enum {
    SOUND_SHOOT,
    SOUND_EXPLOSION,
    SOUND_PICKUP_EXP,
    SOUND_PICKUP_HP,
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
    SPRITESHEET_ICEBALL,
    SPRITESHEET_STARBALL,
    SPRITESHEET_EXP_CRYSTAL,
    SPRITESHEET_HEALTH_CRYSTAL,
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
} ResourceRegistry;

void resourcesInit(ResourceRegistry *resourceRegistry);
