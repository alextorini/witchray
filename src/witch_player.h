#pragma once

#include "smetanka_ecs.h"
#include "witch_collision.h"
#include "witch_components.h"

typedef struct {
    EcsEntityHandle handle;
    FrameCollisionMap collisions;
} Player;

EcsEntityHandle init_player();

void process_input(EcsEntityHandle player_handle, float delta_time);

