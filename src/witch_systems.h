#pragma once

#include "smetanka_ecs.h"

void systemMoveEntities(EcsComponentId positionId, EcsComponentId velocityId, float dt);

void systemMoveParallax(EcsComponentId parallaxId, EcsComponentId positionId, EcsComponentId renderId);

void systemAnimateEntities(EcsComponentId animationId, EcsComponentId renderId, float dt);

void systemRenderEntities(EcsComponentId positionId, EcsComponentId renderId);

void systemRenderText(EcsComponentId positionId, EcsComponentId textRenderId);
