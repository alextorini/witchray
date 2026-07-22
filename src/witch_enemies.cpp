#include <stdint.h>
#include <stdlib.h>
#include "smetanka_ecs.h"
#include "witch_animation.h"
#include "witch_collisions.h"
#include "witch_components.h"
#include "witch_damage.h"
#include "witch_enemies.h"
#include "witch_fire.h"
#include "witch_game.h"
#include "witch_resources.h"

#define SPAWN_COOLDOWN 1.0f
#define ENEMY_SPEED 50.0f
#define MAX_ENEMIES_COUNT 10000
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32

#define ENEMY_DEFAULT_FRAME {0.0f, 0.0f, 32.0f, 32.0f}
#define ENEMY_IDLE_FRAME_TIME 0.5f
#define ENEMY_DYING_FRAME_TIME 0.05f

#define ENEMY_MAX_HEALTH 2.0f

#define ENEMY_FIREBALL_SPEED 150.0f

#define ENM_MALLOC malloc
#define ENM_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define ENM_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define ENM_CALLOC calloc
#define ENM_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define ENM_FREE free

static float spawnCooldown;

static AnimationClip idleAnimation;
static AnimationClip deathAnimation;

static AnimationSet animationSet;

void initEnemyFactory(Game *game) {
    spawnCooldown = SPAWN_COOLDOWN;

    idleAnimation.startFrame = 0;
    idleAnimation.endFrame = 1;
    idleAnimation.frameTime = ENEMY_IDLE_FRAME_TIME;
    idleAnimation.loop = 1;

    deathAnimation.startFrame = 5;
    deathAnimation.endFrame = 9;
    deathAnimation.frameTime = ENEMY_DYING_FRAME_TIME;
    deathAnimation.loop = 0;

    animationSet.clips[ANIMATION_IDLE] = idleAnimation;
    animationSet.clips[ANIMATION_DYING] = deathAnimation;

    game->enemies.deathCooldown = (deathAnimation.endFrame - deathAnimation.startFrame + 1) * ENEMY_DYING_FRAME_TIME;

    initCollisionMap(ENEMY_IMAGE_PATH, (Rectangle)ENEMY_DEFAULT_FRAME, 10, &game->enemies.collisions);
}

static EcsEntityHandle createEnemy(Position *pos, Game *game) {
    EcsEntityHandle handle = ecsCreateEntity();

    ecsAddComponent(handle, game->components[CMP_POSITION], pos);
    Render rndr = {&game->resources.sprites[SPRITESHEET_ENEMY], ENEMY_DEFAULT_FRAME};
    ecsAddComponent(handle, game->components[CMP_RENDER], &rndr);

    Velocity vel = {-ENEMY_SPEED, 0};
    ecsAddComponent(handle, game->components[CMP_VELOCITY], &vel);

    IsEnemy enmy = true;
    ecsAddComponent(handle, game->components[CMP_ENEMY], &enmy);

    Health health = {.current = ENEMY_MAX_HEALTH, .max = ENEMY_MAX_HEALTH};
    ecsAddComponent(handle, game->components[CMP_HEALTH], &health);

    EntityState state = {.id = ENTITY_STATE_IDLE, .cooldown = 0};
    ecsAddComponent(handle, game->components[CMP_ENTITY_STATE], &state);

    EnemyWeaponList weaponList;

    weaponList.weapons[0] = {
        .type = 1,
        .damage = 1.0f,
        .cooldown = 2.0f,
    };

    weaponList.weapons[1] = {0};
    weaponList.weapons[2] = {0};
    ecsAddComponent(handle, game->components[CMP_ENEMY_WEAPON_LIST], &weaponList);

    Animation animation;
    animation.set = &animationSet;
    switchAnimation(&animation, ANIMATION_IDLE);
    ecsAddComponent(handle, game->components[CMP_ANIMATION], &animation);

    return handle;
}

void systemSpawnEnemies(Game *game, float dt) {
    if (game->timer < 5.0f) {
        return;
    }

    if (game->timer > 120.0f) {
        game->enemySpawnCooldown = 0.5f;
    } else if (game->timer > 60.0f) {
        game->enemySpawnCooldown = 1.0f;
    } else if (game->timer > 30.0f) {
        game->enemySpawnCooldown = 1.25f;
    }

    if (spawnCooldown >= 0) {
        spawnCooldown -= dt;

        return;
    }

    if (ecsGetComponentCount(game->components[CMP_ENEMY]) >= MAX_ENEMIES_COUNT) {
        return;
    }

    Position pos;
    pos.x = PLAYSCREEN_WIDTH;
    pos.y = rand() % (PLAYSCREEN_HEIGHT - ENEMY_HEIGHT + 1);

    createEnemy(&pos, game);

    spawnCooldown = game->enemySpawnCooldown;
}

void systemCleanEnemies(Game *game) {
    EcsComponentId enemyId = game->components[CMP_ENEMY];
    EcsComponentId positionId =game->components[CMP_POSITION];

    EcsComponentId componentIdList[] = {enemyId, positionId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);
    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecsGetEntityComponent(positionId, entityHandle);

        if (position->x < -ENEMY_WIDTH) {
            ecsAddToDestroyQueue(entityHandle);
        }
    }

    ecsFlushDestroyQueue();
}

void systemCollideEnemies(Game *game) {
    EcsEntityHandle playerHandle = game->player.handle;
    EcsComponentId enemyId = game->components[CMP_ENEMY];
    EcsComponentId positionId = game->components[CMP_POSITION];
    EcsComponentId renderId = game->components[CMP_RENDER];

    EcsComponentId componentIdList[] = {enemyId, positionId, renderId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 3);

    Position *playerPosition = (Position *)ecsGetEntityComponent(positionId, playerHandle);
    Render *playerRender = (Render *)ecsGetEntityComponent(renderId, playerHandle);
    Animation *playerAnim = (Animation *)ecsGetEntityComponent(game->components[CMP_ANIMATION], playerHandle);
    uint32_t playerFrameIndex = 0;
    if (playerAnim) playerFrameIndex = playerAnim->currentFrame;

    EntityState *playerState = (EntityState *)
        ecsGetEntityComponent(game->components[CMP_ENTITY_STATE], game->player.handle);

    EcsEntityHandle enemyHandle;
    while ((enemyHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *enemyState = (EntityState *)ecsGetEntityComponent(game->components[CMP_ENTITY_STATE], enemyHandle);
        if (enemyState->id != ENTITY_STATE_IDLE || playerState->id != ENTITY_STATE_IDLE) {
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
            &game->player.collisions, playerFrameIndex, playerPosition->x, playerPosition->y,
            &game->enemies.collisions, enemyFrameIndex, enemyPosition->x, enemyPosition->y
        )) {

            damage(playerHandle, 1000000.0f, game);

            return;
        }
    }
}

void systemEnemiesFire(Game *game, float dt) {
    EcsEntityHandle playerHandle = game->player.handle;
    EcsComponentId enemyId = game->components[CMP_ENEMY];
    EcsComponentId positionId = game->components[CMP_POSITION];
    EcsComponentId renderId = game->components[CMP_RENDER];
    EcsComponentId eWeaponListId = game->components[CMP_ENEMY_WEAPON_LIST];

    EcsComponentId componentIdList[] = {enemyId, positionId, renderId, eWeaponListId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 4);

    Position *playerPosition = (Position *)ecsGetEntityComponent(positionId, playerHandle);

    EcsEntityHandle enemyHandle;
    while ((enemyHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *enemyState = (EntityState *)ecsGetEntityComponent(game->components[CMP_ENTITY_STATE], enemyHandle);
        if (enemyState->id != ENTITY_STATE_IDLE) {
            continue;
        }

        Position *enemyPosition = (Position *)ecsGetEntityComponent(positionId, enemyHandle);
        Render *enemyRender = (Render *)ecsGetEntityComponent(renderId, enemyHandle);
        Animation *enemyAnim = (Animation *)ecsGetEntityComponent(game->components[CMP_ANIMATION], enemyHandle);
        EnemyWeaponList *eWeaponList =
            (EnemyWeaponList *)ecsGetEntityComponent(game->components[CMP_ENEMY_WEAPON_LIST], enemyHandle);

        for (int i = 0; i < 3; i++) {
            if (eWeaponList->weapons[i].type == 0) {
                continue;
            }

            if (eWeaponList->weapons[i].cooldown > 0) {
                eWeaponList->weapons[i].cooldown -= dt;
                continue;
            }

            float x = playerPosition->x - enemyPosition->x;
            float y = playerPosition->y - enemyPosition->y;
            float distance = xyMagnitude(x, y);

            Velocity velocity = {x / distance * ENEMY_FIREBALL_SPEED, y / distance * ENEMY_FIREBALL_SPEED};

            createFireball(enemyPosition, &velocity, CASTER_ENEMY, game);
            eWeaponList->weapons[i].cooldown = 3;
        }

    }
}

