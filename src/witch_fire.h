#pragma once

#include "smetanka_ecs.h"
#include "witch_game.h"

void init_fireballs(Game *game);

void cast_fireballs(Game *game, float dt);

void system_fireballs_collide_enemies(Game *game);

void system_clean_fireballs(EcsComponentId fireball_id, EcsComponentId position_id);
