#include <stdint.h>
#include <stdlib.h>
#include "smetanka_ecs.h"
#include "witch_collisions.h"
#include "witch_components.h"
#include "witch_enemies.h"
#include "witch_fire.h"
#include "witch_game.h"
#include "witch_resources.h"
#include "witch_save.h"

#define SPAWN_COOLDOWN 0.6
#define ENEMY_SPEED 50.0
#define MAX_ENEMIES_COUNT 10000
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32

#define ENEMY_DEFAULT_FRAME {0.0f, 0.0f, 32.0f, 32.0f}
#define ENEMY_FRAME_TIME 0.5

#define ENEMY_FIREBALL_SPEED 150.0f

#define ENM_MALLOC malloc
#define ENM_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define ENM_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define ENM_CALLOC calloc
#define ENM_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define ENM_FREE free

static uint16_t enemiesCount;
static float spawnCooldown;

static AnimationClip idleAnimation;
static AnimationSet animationSet;

void initEnemyFactory(Game *game) {
    enemiesCount = 0;
    spawnCooldown = SPAWN_COOLDOWN;

    idleAnimation.startFrame = 0;
    idleAnimation.endFrame = 1;
    idleAnimation.frameTime = ENEMY_FRAME_TIME;
    idleAnimation.loop = 1;
    animationSet.clips = ENM_MALLOC_TYPE(AnimationClip);
    animationSet.count = 1;
    animationSet.clips[0] = idleAnimation;

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
    animation.currentClip = 0;
    animation.currentFrame = 0;
    animation.timer = 0.0;
    ecsAddComponent(handle, game->components[CMP_ANIMATION], &animation);

    enemiesCount++;

    return handle;
}

void systemSpawnEnemies(Game *game, float dt) {
    if (spawnCooldown >= 0) {
        spawnCooldown -= dt;

        return;
    }

    if (enemiesCount >= MAX_ENEMIES_COUNT) {
        return;
    }

    Position pos;
    pos.x = VIRTUAL_WIDTH;
    pos.y = rand() % (VIRTUAL_HEIGHT - ENEMY_HEIGHT + 1);

    createEnemy(&pos, game);

    spawnCooldown = SPAWN_COOLDOWN;
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
            ecsDestroyEntity(entityHandle);
        }
    }
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

    EcsEntityHandle enemyHandle;
    while ((enemyHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
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
            saveGame(game);
            game->shouldClose = 1;

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

