#pragma once

#include "smetanka_ecs.h"
#include "witch_collision.h"
#include "witch_components.h"

typedef struct {
    FrameCollisionMap collisions;
} Enemies;

void init_enemy_factory();

void system_spawn_enemies(float dt);

void system_clean_enemies(EcsComponentId enemy_id, EcsComponentId position_id);

void system_collide_enemies(
    EcsEntityHandle player_handle,
    EcsComponentId enemy_id,
    EcsComponentId position_id,
    EcsComponentId render_id
);
