#pragma once

#include "witch_game.h"
#include "witch_input_read.h"

void init_start_screen(Game *game);

void start_screen_update(InputState *input, Game *game, float dt);

void destroy_start_screen();
