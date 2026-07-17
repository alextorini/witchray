#include "witch_fire.h"
#include "ext/raylib.h"
#include "ext/raymath.h"
#include "smetanka_ecs.h"
#include "witch_collisions.h"
#include "witch_components.h"
#include "witch_game.h"
#include "witch_resources.h"
#include "witch_save.h"

#define FIREBALL_DEFAULT_FRAME {0.0f, 0.0f, 5.0f, 5.0f}
#define FIREBALL_CAST_POSITION {38.0f, 18.0f}
#define FIREBALL_COOLDOWN 0.4
#define FIREBALL_SPEED 300.0

static float fireballCooldown = 0.0;

void initFireballs(Game *game) {
    game->fireballs.deathCooldown = 0.0f;
    initCollisionMap(FIREBALL_IMAGE_PATH, (Rectangle)FIREBALL_DEFAULT_FRAME, 10, &game->fireballs.collisions);
}

void createFireball(Position *position, Velocity *velocity, Caster caster, Game *game) {
    EcsEntityHandle handle = ecsCreateEntity();
    ecsAddComponent(handle, game->components[CMP_POSITION], position);
    ecsAddComponent(handle, game->components[CMP_VELOCITY], velocity);

    Fireball fireball = {.damage = 1, .caster = caster};
    ecsAddComponent(handle, game->components[CMP_FIREBALL], &fireball);

    Render rndr = {&game->resources.sprites[SPRITESHEET_FIREBALL], FIREBALL_DEFAULT_FRAME};
    ecsAddComponent(handle, game->components[CMP_RENDER], &rndr);

    EntityState state = {.id = ENTITY_STATE_IDLE, .cooldown = 0.0f};
    ecsAddComponent(handle, game->components[CMP_ENTITY_STATE], &state);
}

void castFireballs(Game *game, float dt) {
    Position *playerPosition = (Position *)ecsGetEntityComponent(game->components[CMP_POSITION], game->player.handle);
    if (fireballCooldown <= 0) {
        Position fireballPosition = Vector2Add(*playerPosition, (Position)FIREBALL_CAST_POSITION);
        Velocity fireballVelocity = {FIREBALL_SPEED, 0};
        createFireball(&fireballPosition, &fireballVelocity, CASTER_PLAYER, game);
        fireballCooldown = FIREBALL_COOLDOWN;

        return;
    }

    fireballCooldown -= dt;
}

void systemFireballsCollide(Game *game) {
    EcsComponentId fireballId = game->components[CMP_FIREBALL];
    EcsComponentId enemyId = game->components[CMP_ENEMY];
    EcsComponentId positionId = game->components[CMP_POSITION];
    EcsComponentId renderId = game->components[CMP_RENDER];

    EcsComponentId enemyComponentIdList[] = {enemyId, positionId, renderId};
    EcsEntityIterator enemyIterator = ecsGetEntityIterator(enemyComponentIdList, 3);

    EcsComponentId fireballComponentIdList[] = {fireballId, positionId, renderId};
    EcsEntityIterator fireballIterator = ecsGetEntityIterator(fireballComponentIdList, 3);

    EcsEntityHandle fireballHandle;
    EcsEntityHandle enemyHandle;
    while ((fireballHandle = ecsGetNextEntity(&fireballIterator)) != INVALID_HANDLE) {
        EntityState *fireballState =
            (EntityState *)ecsGetEntityComponent(game->components[CMP_ENTITY_STATE], fireballHandle);
        if (fireballState->id != ENTITY_STATE_IDLE) {
            continue;
        }

        Fireball *fireball = (Fireball *)ecsGetEntityComponent(fireballId, fireballHandle);

        Position *fireballPosition = (Position *)ecsGetEntityComponent(positionId, fireballHandle);
        Render *fireballRender = (Render *)ecsGetEntityComponent(renderId, fireballHandle);
        Animation *fireballAnim =
            (Animation *)ecsGetEntityComponent(game->components[CMP_ANIMATION], fireballHandle);
        uint32_t fireballFrameIndex = 0;
        if (fireballAnim) fireballFrameIndex = fireballAnim->currentFrame;

        if (fireball->caster == CASTER_ENEMY) {
            Animation *playerAnim = (Animation *)ecsGetEntityComponent(game->components[CMP_ANIMATION], game->player.handle);
            Position *playerPos = (Position *)ecsGetEntityComponent(game->components[CMP_POSITION], game->player.handle);
            if(checkCollision(
                &game->fireballs.collisions, fireballFrameIndex, fireballPosition->x, fireballPosition->y,
                &game->player.collisions, playerAnim->currentFrame, playerPos->x, playerPos->y
            )) {
                saveGame(game);
                game->shouldClose = 1;

                return;
            }

            continue;
        }

        EcsEntityIterator enemyIterator = ecsGetEntityIterator(enemyComponentIdList, 3);
        while ((enemyHandle = ecsGetNextEntity(&enemyIterator)) != INVALID_HANDLE) {
            EntityState *enemyState =
                (EntityState *)ecsGetEntityComponent(game->components[CMP_ENTITY_STATE], enemyHandle);
            if (enemyState->id != ENTITY_STATE_IDLE) {
                continue;
            }

            Position *enemyPosition = (Position *)ecsGetEntityComponent(positionId, enemyHandle);
            Render *enemyRender = (Render *)ecsGetEntityComponent(renderId, enemyHandle);
            Animation *enemyAnim = (Animation *)ecsGetEntityComponent(game->components[CMP_ANIMATION], enemyHandle);

            uint32_t enemyFrameIndex = 0;
            if (enemyAnim) {
                enemyFrameIndex = enemyAnim->currentFrame;
            }

            if (checkCollision(
                &game->fireballs.collisions, fireballFrameIndex, fireballPosition->x, fireballPosition->y,
                &game->enemies.collisions, enemyFrameIndex, enemyPosition->x, enemyPosition->y
            )) {
                enemyState->id = ENTITY_STATE_DYING;
                enemyState->cooldown = game->enemies.deathCooldown;

                enemyAnim->currentClip = 1;
                enemyAnim->currentFrame = 5;
                enemyAnim->timer = 0.0;

                fireballState->id = ENTITY_STATE_DIE;

                game->enemiesKilled++;

                break;
            }
        }
    }
}

void systemCleanFireballs(EcsComponentId fireballId, EcsComponentId positionId, EcsComponentId entityStateComponentId) {
    EcsComponentId componentIdList[] = {fireballId, positionId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);
    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *fireballState = (EntityState *)ecsGetEntityComponent(entityStateComponentId, entityHandle);
        if (fireballState->id == ENTITY_STATE_DIE) {
            continue;
        }

        Position *position = (Position *)ecsGetEntityComponent(positionId, entityHandle);

        if (position->x > VIRTUAL_WIDTH || position->x < -(Rectangle)FIREBALL_DEFAULT_FRAME.width
            || position->y > VIRTUAL_HEIGHT || position->y < -(Rectangle)FIREBALL_DEFAULT_FRAME.height
        ) {
            fireballState->id = ENTITY_STATE_DIE;
        }
    }
}
