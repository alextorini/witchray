#pragma once

#include <stdint.h>
#include "witch_game.h"

void init(Game *game);

void update_and_draw(Game *game, float delta_time);

void unload(Game *game);
