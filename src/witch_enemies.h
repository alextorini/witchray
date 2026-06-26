#pragma once

#include "smetanka_ecs.h"
#include "witch_core.h"

void init_enemy_factory();

void system_spawn_enemies(float dt);

void system_clean_enemies(EcsComponentId enemy_id, EcsComponentId position_id);

void system_collide_enemies(
    EcsEntityHandle player_handle,
    EcsComponentId enemy_id,
    EcsComponentId position_id,
    EcsComponentId render_id
);
