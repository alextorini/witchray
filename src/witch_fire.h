#pragma once

#include "smetanka_ecs.h"
#include "witch_game.h"

void initFireballs(Game *game);

void castFireballs(Game *game, float dt);

void systemFireballsCollideEnemies(Game *game);

void systemCleanFireballs(EcsComponentId fireballId, EcsComponentId positionId);
