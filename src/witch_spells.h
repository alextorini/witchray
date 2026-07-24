#pragma once

#include "smetanka_ecs.h"
#include "witch_components.h"
#include "witch_game.h"

void initFireballs(Game *game);

void createFireball(Position *position, Velocity *velocity, EcsEntityHandle caster, Game *game);

void castPlayerSpells(Game *game, float dt);

void systemCollideSpells(Game *game);

void systemCleanFireballs();
