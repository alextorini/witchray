#include <stdint.h>

#include "ext/raylib.h"
#include "smetanka_ecs.h"
#include "witch_systems.h"
#include "smetanka_engine.h"
#include "smetanka_math.h"
#include "witch_components.h"
#include "witch_game.h"
#include "witch_save.h"
#include "witch_state.h"

void systemMoveEntities(float dt) {
    static EcsComponentId componentIdList[] = {COMPONENT_ID(Position), COMPONENT_ID(Velocity)};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Position *position = getPosition(entityHandle);
        Velocity *velocity =  getVelocity(entityHandle);

        *position = toPosition(
            vector2Sum(
                toVector2(*position),
                vector2Scale(toVector2(*velocity), dt)
            )
        );
    }
}

void systemMoveParallax() {
    static EcsComponentId componentIdList[] = {
        COMPONENT_ID(Parallax),
        COMPONENT_ID(Position),
        COMPONENT_ID(Render)
    };
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 3);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Position *position = getPosition(entityHandle);
        Render *render = getRender(entityHandle);

        if (position->x + render->frame.width < 0) {
            position->x += render->frame.width * 2.0f;
        }
    }
}

void systemAnimateEntities(float dt) {
    EcsComponentId componentIdList[] = {COMPONENT_ID(Animation), COMPONENT_ID(Render)};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *entityState = getEntityState(entityHandle);
        if (entityState && entityState->id == ENTITY_STATE_DIE) {
            continue;
        }

        Animation *animation = getAnimation(entityHandle);
        Render *render = getRender(entityHandle);

        AnimationClip *currentClip = &animation->set->clips[animation->currentClip];
        animation->timer += dt;

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

void systemRenderEntities(ResourceMap *resources) {
    EcsComponentId componentIdList[] = {COMPONENT_ID(Position), COMPONENT_ID(Render)};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *entityState = getEntityState(entityHandle);
        if (entityState && entityState->id == ENTITY_STATE_DIE) {
            continue;
        }

        Position *position = getPosition(entityHandle);
        Render *render = getRender(entityHandle);

        float flashColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        SetShaderValue(
            resources->shaders.damageFlash.shader,
            resources->shaders.damageFlash.colorLoc,
            flashColor,
            SHADER_UNIFORM_VEC4
        );

        float flashStrength = (entityState)  ? entityState->cooldown * 4 : 0.0f;
        SetShaderValue(
            resources->shaders.damageFlash.shader,
            resources->shaders.damageFlash.strengthLoc,
            &flashStrength,
            SHADER_UNIFORM_FLOAT
        );
        BeginShaderMode(resources->shaders.damageFlash.shader);
        smeDrawTextureRec(*render->spritesheet, render->frame, toVector2(*position), WHITE);
        EndShaderMode();
    }
}

void systemRenderText() {
    EcsComponentId componentIdList[] = {COMPONENT_ID(Position), COMPONENT_ID(TextRender)};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Position *position = getPosition(entityHandle);
        TextRender *textRender = getTextRender(entityHandle);

        smeDrawText(
            textRender->font,
            textRender->text,
            vector2Sum(toVector2(*position), toVector2(textRender->offset)),
            textRender->fontSize,
            textRender->spacing,
            textRender->color
        );
    }
}

void systemProcessEntityStates(Game *game) {
    EcsComponentId componentIdList[] = {COMPONENT_ID(EntityState)};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 1);

    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *entityState = getEntityState(entityHandle);
        entityState->cooldown -= smeGetFrameTime();
        if (entityState->cooldown < 0.0f) {
            entityState->cooldown = 0.0f;
        };

        if (entityState->id == ENTITY_STATE_DYING) {
            if (entityState->cooldown <= 0.0f) {
                entityState->id = ENTITY_STATE_DIE;
            }
        }

        if (entityState->id == ENTITY_STATE_DIE) {
            if (entityHandle == game->player.handle) {
                saveGame(game);
                stateRequestChange(game, STATE_START_SCREEN);
            }
        }
    }
}
