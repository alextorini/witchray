#pragma once

#include "witch_game.h"

void initEnemyFactory(Game *game);

void systemSpawnEnemies(Game *game, float dt);

void systemCleanEnemies();

void systemCollideEnemies(Game *game);

void systemEnemiesFire(Game *game, float dt);
