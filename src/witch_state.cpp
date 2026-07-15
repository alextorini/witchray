#include "smetanka_misc.h"
#include "witch_game.h"
#include "witch_gameplay.h"
#include "witch_start.h"

void state_change(Game *game, int state) {
    switch (game->state) {
        case STATE_START_SCREEN: {
            if (state != STATE_GAMEPLAY) {
                ABORT();
            }

            destroy_start_screen();

            gameplay_init(game);

            game->state = state;
        }
    }

}
