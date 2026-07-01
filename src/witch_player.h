#pragma once

#include "smetanka_ecs.h"

EcsEntityHandle init_player();

void process_input(EcsEntityHandle player_handle, float delta_time);

