#pragma once

#include "witch_game.h"

void systemMoveEntities(float dt);

void systemMoveParallax();

void systemAnimateEntities(float dt);

void systemRenderEntities(ResourceMap *resources);

void systemRenderText();

void systemProcessEntityStates(Game *game);
