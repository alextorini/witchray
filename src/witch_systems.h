#pragma once

#include "witch_game.h"

void systemMoveEntities(float dt);

void systemMoveParallax();

void systemAnimateEntities(float dt);

void systemRenderEntities(ResourceRegistry *resources);

void systemRenderText();

void systemProcessEntityStates(Game *game);
