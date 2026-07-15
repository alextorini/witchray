#pragma once

#include <stdint.h>
#include "witch_game.h"

void init(Game *game);

void updateAndDraw(Game *game, float deltaTime);

void unload(Game *game);
