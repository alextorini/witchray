#pragma once

#include "smetanka_ecs.h"

void system_move_entities(EcsComponentId position_id, EcsComponentId velocity_id, float delta_time);

void system_move_parallax(EcsComponentId parallax_id, EcsComponentId position_id, EcsComponentId render_id);

void system_animate_entities(EcsComponentId animation_id, EcsComponentId render_id, float delta_time);

void system_render_entities(EcsComponentId position_id, EcsComponentId render_id);
