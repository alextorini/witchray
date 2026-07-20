#include "smetanka_misc.h"
#include "witch_game.h"
#include "witch_gameplay.h"
#include "witch_start.h"

void stateChange(Game *game, int state) {
    switch (game->state) {
        case STATE_START_SCREEN: {
            if (state != STATE_GAMEPLAY) {
                ABORT();
            }

            destroyStartScreen();

            gameplayInit(game);

            game->state = state;

            break;
        }
        case STATE_GAMEPLAY: {
            if (state != STATE_START_SCREEN) {
                ABORT();
            }

            gameplayDestroy(game);

            startScreenInit(game);

            game->state = state;

            break;
        }
    }

}
