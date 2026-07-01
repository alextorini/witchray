#pragma once

#include "smetanka_ecs.h"
#include "witch_components.h"

void init_fireballs();

void cast_fireballs(EcsEntityHandle player_handle, float delta_time);

void system_fireballs_collide_enemies(
    EcsComponentId fireball_id,
    EcsComponentId enemy_id,
    EcsComponentId position_id,
    EcsComponentId render_id
);
