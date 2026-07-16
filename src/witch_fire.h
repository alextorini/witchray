#pragma once

#include "smetanka_ecs.h"
#include "witch_game.h"

void initFireballs(Game *game);

void createFireball(Position *position, Velocity *velocity, Caster caster, Game *game);

void castFireballs(Game *game, float dt);

void systemFireballsCollide(Game *game);

void systemCleanFireballs(EcsComponentId fireballId, EcsComponentId positionId);
