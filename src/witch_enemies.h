#pragma once

#include "witch_game.h"

void init_enemy_factory(Game *game);

void system_spawn_enemies(Game *game, float dt);

void system_clean_enemies(Game *game);

void system_collide_enemies(Game *game);
