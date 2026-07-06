
#include "smetanka_misc.h"
#include "witch_game_update.h"
#include "witch_start.h"
#include "witch_gameplay.h"

void game_update(InputState *input, Game *game, float dt) {
    UpdateMusicStream(game->resources.music[MUSIC_MAIN]);
    ClearBackground(SKY_COLOR);

    switch (game->state) {
        case STATE_START_SCREEN:
            start_screen_update(input, game, dt);
            break;
        case STATE_GAMEPLAY:
            gameplay_update(input, game, dt);
            break;
        default:
            ABORT();
    }
}

