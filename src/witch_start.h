#pragma once

#include "witch_game.h"
#include "witch_input_read.h"

void startScreenInit(Game *game);

void startScreenUpdate(InputState *input, Game *game, float dt);

void destroyStartScreen();
