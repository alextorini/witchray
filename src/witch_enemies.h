#pragma once

#include "witch_core.h"

void init_enemy_factory();

void system_spawn_enemies(float dt);

void system_clean_enemies(EcsComponentId enemy_id, EcsComponentId position_id);
