#include "smetanka_misc.h"
#include "witch_game.h"
#include "witch_gameplay.h"
#include "witch_start.h"

void stateRequestChange(Game *game, int state) {
    switch (game->state) {
        case STATE_START_SCREEN: {
            if (state != STATE_GAMEPLAY) {
                ABORT();
            }

            game->requestedState = state;

            break;
        }
        case STATE_GAMEPLAY: {
            if (state != STATE_START_SCREEN) {
                ABORT();
            }

            game->requestedState = state;

            break;
        }
    }
}

void stateUpdate(Game *game) {
    if (game->state == game->requestedState) {
        return;
    }

    switch (game->state) {
        case STATE_START_SCREEN: {
            if (game->requestedState != STATE_GAMEPLAY) {
                ABORT();
            }

            startScreenDestroy();

            gameplayInit(game);

            game->state = game->requestedState;

            break;
        }
        case STATE_GAMEPLAY: {
            if (game->requestedState != STATE_START_SCREEN) {
                ABORT();
            }


            gameplayDestroy(game);

            startScreenInit(game);

            game->state = game->requestedState;

            break;
        }
    }

}
