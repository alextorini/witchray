#pragma once

#include "witch_game.h"
#include "witch_input_read.h"

void gameplayInit(Game *game);

void gameplayUpdate(InputState *input, Game *game, float dt);

void gameplayDestroy(Game *game);
