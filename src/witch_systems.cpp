#include <stdint.h>

#include "smetanka_ecs.h"
#include "witch_systems.h"
#include "smetanka_engine.h"
#include "smetanka_math.h"
#include "witch_components.h"

void systemMoveEntities(EcsComponentId positionId, EcsComponentId velocityId, float dt) {
    EcsComponentId componentIdList[] = {positionId, velocityId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecsGetEntityComponent(positionId, entityHandle);
        Velocity *velocity = (Velocity *)ecsGetEntityComponent(velocityId, entityHandle);

        *position = vector2Sum(*position, vector2Scale(*velocity, dt));
    }
}

void systemMoveParallax(EcsComponentId parallaxId, EcsComponentId positionId, EcsComponentId renderId) {
    EcsComponentId componentIdList[] = {parallaxId, positionId, renderId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 3);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecsGetEntityComponent(positionId, entityHandle);
        Render *render = (Render *)ecsGetEntityComponent(renderId, entityHandle);

        if (position->x + render->frame.width < 0) {
            position->x += render->frame.width * 2.0f;
        }
    }
}

void systemAnimateEntities(
    EcsComponentId animation_id,
    EcsComponentId render_id,
    EcsComponentId entityStateComponentId,
    float delta_time
) {
    EcsComponentId componentIdList[] = {animation_id, render_id};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *entityState = (EntityState *)ecsGetEntityComponent(entityStateComponentId, entityHandle);
        if (entityState && entityState->id == ENTITY_STATE_DIE) {
            continue;
        }

        Animation *animation = (Animation*)ecsGetEntityComponent(animation_id, entityHandle);
        Render *render = (Render *)ecsGetEntityComponent(render_id, entityHandle);

        AnimationClip *currentClip = &animation->set->clips[animation->currentClip];
        animation->timer += delta_time;

        if (currentClip->frameTime <= 0.00001f) return;

        while (animation->timer >= currentClip->frameTime) {
            animation->currentFrame++;
            if (animation->currentFrame > currentClip->endFrame) {
                animation->currentFrame = currentClip->startFrame;
            }

            animation->timer -= currentClip->frameTime;
        }

        render->frame.x = (int)(render->frame.width * animation->currentFrame) % render->spritesheet->width;
        render->frame.y = (/*NOLINT*/(int)(render->frame.width * animation->currentFrame) /
            render->spritesheet->width) * render->frame.height;
    }
}

void systemRenderEntities(EcsComponentId positionId, EcsComponentId renderId, EcsComponentId entityStateComponentId) {
    EcsComponentId componentIdList[] = {positionId, renderId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *entityState = (EntityState *)ecsGetEntityComponent(entityStateComponentId, entityHandle);
        if (entityState && entityState->id == ENTITY_STATE_DIE) {
            continue;
        }

        Position *position = (Position*)ecsGetEntityComponent(positionId, entityHandle);
        Render *render = (Render *)ecsGetEntityComponent(renderId, entityHandle);

        smeDrawTextureRec(*render->spritesheet, render->frame, *position, WHITE);
    }
}

void systemRenderText(EcsComponentId positionId, EcsComponentId textRenderId) {
    EcsComponentId componentIdList[] = {positionId, textRenderId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position*)ecsGetEntityComponent(positionId, entityHandle);
        TextRender *textRender = (TextRender *)ecsGetEntityComponent(textRenderId, entityHandle);

        smeDrawText(
            textRender->font,
            textRender->text,
            vector2Sum(*position, textRender->offset),
            textRender->fontSize,
            textRender->spacing,
            textRender->color
        );
    }
}

void systemDestroyEntities(EcsComponentId entityStateComponentId) {
    EcsComponentId componentIdList[] = {entityStateComponentId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 1);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *entityState = (EntityState *)ecsGetEntityComponent(entityStateComponentId, entityHandle);
        if (entityState->id == ENTITY_STATE_DYING) {
            entityState->cooldown -= smeGetFrameTime();
            if (entityState->cooldown <= 0.0f) {
                entityState->id = ENTITY_STATE_DIE;
            }
        }

        if (entityState->id == ENTITY_STATE_DIE) {
            ecsDestroyEntity(entityHandle);
        }
    }
}
