#pragma once

#include <stdint.h>
#include "ext/raylib.h"
#include "witch_game.h"

void init(Game *game);

void updateAndRender(Game *game, RenderTexture *screen, float deltaTime);

void unload(Game *game);
