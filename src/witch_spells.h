#pragma once

#include "smetanka_ecs.h"
#include "witch_components.h"
#include "witch_game.h"

void initFireballs(Game *game);

void createFireball(Position *position, Velocity *velocity, EcsEntityHandle caster, float damage, Game *game);

void castPlayerSpells(Game *game, float dt);

void systemHomingVelocityUpdate();

void systemCollideSpells(Game *game);

void systemCleanFireballs();
