#include "witch_game.h"
#include "witch_event.h"
#include "witch_state.h"
#include <cstdint>

typedef enum {
    TIME_ENEMY_SPAWN,
    TIME_SPELL_CAST,
    TIME_INC_1,
    TIME_INC_2,
    TIME_INC_3,
    TIME_WIN,
    TIME_END,
    TIME_COUNT
} TimeEventIndex;

static float timePoints[TIME_COUNT];
static int8_t currentTimePoint = 0;

static void timeEventCreate(TimeEventIndex event, Game *game);

void timeInit(Game *game) {
    game->timer = 0.0f;

    timePoints[TIME_ENEMY_SPAWN] = 3.0f;
    timePoints[TIME_SPELL_CAST] = 5.0f;
    timePoints[TIME_INC_1] = 30.0f;
    timePoints[TIME_INC_2] = 60.0f;
    timePoints[TIME_INC_3] = 90.0f;
    timePoints[TIME_WIN] = 180.0f;
    timePoints[TIME_END] = 200.0f;
}

void timeUpdate(Game *game, float dt) {
    game->timer += dt;

    if (game->timer >= timePoints[currentTimePoint]) {
        timeEventCreate((TimeEventIndex)currentTimePoint, game);
        currentTimePoint++;
    }
}

static void timeEventCreate(TimeEventIndex event, Game *game) {
    switch (event) {
        case TIME_ENEMY_SPAWN: {
            game->spawnEnemies = 1;
        } break;
        case TIME_SPELL_CAST: {
            game->castSpells = 1;
        } break;
        case TIME_INC_1: {
            game->skyColor = SKY_COLOR_2;
            game->enemySpawnCooldown = SPAWN_COOLDOWN * 0.90f;
        } break;
        case TIME_INC_2: {
            game->enemySpawnCooldown = SPAWN_COOLDOWN * 0.80f;
        } break;
        case TIME_INC_3: {
            game->enemySpawnCooldown = SPAWN_COOLDOWN * 0.70f;
        } break;
        case TIME_WIN: {
            eventCreate(game->player.handle, EVENT_WIN, game);
        } break;
        case TIME_END: {
            stateRequestChange(game, STATE_START_SCREEN);
        } break;
        case TIME_COUNT: {};
    }
}
