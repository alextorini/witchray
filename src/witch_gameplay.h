#pragma once

#include "witch_game.h"
#include "witch_input_read.h"

void gameplay_init(Game *game);

void gameplay_update(InputState *input, Game *game, float dt);

void gameplay_destroy(Game *game);
