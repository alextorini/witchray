
#include "ext/raylib.h"
#include "smetanka_engine.h"
#include "smetanka_misc.h"
#include "witch_game_update.h"
#include "witch_resources.h"
#include "witch_start.h"
#include "witch_gameplay.h"

void gameUpdate(InputState *input, Game *game, float dt) {
    if (IsMusicStreamPlaying(game->resources.music[MUSIC_MAIN])) {
        smeUpdateMusicStream(game->resources.music[MUSIC_MAIN]);
    }

    switch (game->state) {
        case STATE_START_SCREEN:
            startScreenUpdate(input, game, dt);
            break;
        case STATE_GAMEPLAY:
            gameplayUpdate(input, game, dt);
            break;
        default:
            ABORT();
    }
}

