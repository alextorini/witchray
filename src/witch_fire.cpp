#include "witch_fire.h"
#include "ext/raylib.h"
#include "ext/raymath.h"
#include "smetanka_ecs.h"
#include "witch_components.h"
#include "witch_game.h"
#include "witch_resources.h"

#define FIREBALL_DEFAULT_FRAME {0.0f, 0.0f, 3.0f, 3.0f}
#define FIREBALL_CAST_POSITION {38.0f, 18.0f}
#define FIREBALL_COOLDOWN 0.4
#define FIREBALL_SPEED 300.0

static float fireballCooldown = 0.0;

void initFireballs(Game *game) {
    initCollisionMap(FIREBALL_IMAGE_PATH, (Rectangle)FIREBALL_DEFAULT_FRAME, 10, &game->fireballs.collisions);
}

static void createFireball(Position *position, Velocity *velocity, Game *game) {
    EcsEntityHandle handle = ecsCreateEntity();
    ecsAddComponent(handle, game->components[CMP_POSITION], position);
    ecsAddComponent(handle, game->components[CMP_VELOCITY], velocity);

    Fireball fireball = {1};
    ecsAddComponent(handle, game->components[CMP_FIREBALL], &fireball);

    Render rndr = {&game->resources.sprites[SPRITESHEET_FIREBALL], FIREBALL_DEFAULT_FRAME};
    ecsAddComponent(handle, game->components[CMP_RENDER], &rndr);
}

void castFireballs(Game *game, float dt) {
    Position *playerPosition = (Position *)ecsGetEntityComponent(game->components[CMP_POSITION], game->player.handle);
    if (fireballCooldown <= 0) {
        Position fireballPosition = Vector2Add(*playerPosition, (Position)FIREBALL_CAST_POSITION);
        Velocity fireballVelocity = {FIREBALL_SPEED, 0};
        createFireball(&fireballPosition, &fireballVelocity, game);
        fireballCooldown = FIREBALL_COOLDOWN;

        return;
    }

    fireballCooldown -= dt;
}

void systemFireballsCollideEnemies(Game *game) {
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
        Position *fireballPosition = (Position *)ecsGetEntityComponent(positionId, fireballHandle);
        Render *fireballRender = (Render *)ecsGetEntityComponent(renderId, fireballHandle);
        Animation *fireballAnim =
            (Animation *)ecsGetEntityComponent(game->components[CMP_ANIMATION], fireballHandle);
        uint32_t fireballFrameIndex = 0;
        if (fireballAnim) fireballFrameIndex = fireballAnim->currentFrame;
        EcsEntityIterator enemyIterator = ecsGetEntityIterator(enemyComponentIdList, 3);
        while ((enemyHandle = ecsGetNextEntity(&enemyIterator)) != INVALID_HANDLE) {
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
                // TODO: implement delayed destroy
                ecsDestroyEntity(enemyHandle);
                ecsDestroyEntity(fireballHandle);
                game->enemiesKilled++;
                break;
            }
        }
    }
}

void systemCleanFireballs(EcsComponentId fireballId, EcsComponentId positionId) {
    EcsComponentId componentIdList[] = {fireballId, positionId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);
    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecsGetEntityComponent(positionId, entityHandle);

        if (position->x > VIRTUAL_WIDTH) {
            ecsDestroyEntity(entityHandle);
        }
    }
}
