#pragma once

#include "smetanka_ecs.h"

void systemMoveEntities(EcsComponentId positionId, EcsComponentId velocityId, float dt);

void systemMoveParallax(EcsComponentId parallaxId, EcsComponentId positionId, EcsComponentId renderId);

void systemAnimateEntities(
    EcsComponentId animation_id,
    EcsComponentId render_id,
    EcsComponentId entityStateComponentId,
    float delta_time
);

void systemRenderEntities(EcsComponentId positionId, EcsComponentId renderId, EcsComponentId entityStateComponentId);

void systemRenderText(EcsComponentId positionId, EcsComponentId textRenderId);

void systemDestroyEntities(EcsComponentId entityStateComponentId);
