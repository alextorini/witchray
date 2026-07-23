#pragma once

#include "witch_components.h"
#include "witch_game.h"

void initFireballs(Game *game);

void createFireball(Position *position, Velocity *velocity, Caster caster, Game *game);

void castPlayerSpells(Game *game, float dt);

void systemFireballsCollide(Game *game);

void systemCleanFireballs();
