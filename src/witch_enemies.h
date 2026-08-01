#pragma once

#include "witch_game.h"

void initEnemyFactory(Game *game);

void enemiesSpawn(Game *game, float dt);

void systemEnemiesClean();

void systemEnemiesCollide(Game *game);

void systemEnemiesFire(Game *game, float dt);
