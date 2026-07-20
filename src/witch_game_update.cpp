
#include "smetanka_misc.h"
#include "witch_game_update.h"
#include "witch_start.h"
#include "witch_gameplay.h"

void gameUpdate(InputState *input, Game *game, float dt) {
    UpdateMusicStream(game->resources.music[MUSIC_MAIN]);

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

