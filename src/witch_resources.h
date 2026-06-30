#pragma once

#include "ext/raylib.h"

#define PLAYER_IMAGE_PATH "images/player.png"
#define ENEMY_IMAGE_PATH "images/enemy.png"

typedef enum {
    SOUND_SHOOT,
    SOUND_EXPLOSION,
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
    SPRITE_COUNT
} SpriteIndex;

typedef enum {
    FONT_MAIN,
    FONT_COUNT
} FontIndex;

typedef struct {
    Font fonts[FONT_COUNT];
    Texture sprites[SPRITE_COUNT];
    Sound sounds[SOUND_COUNT];
    Music music[MUSIC_COUNT];
} ResourceMap;

void init_resources(ResourceMap *resource_map);
