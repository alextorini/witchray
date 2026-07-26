#include "witch_spells.h"
#include "ext/raylib.h"
#include "smetanka_ecs.h"
#include "smetanka_math.h"
#include "witch_collisions.h"
#include "witch_components.h"
#include "witch_damage.h"
#include "witch_event.h"
#include "witch_game.h"
#include "witch_resources.h"

#define FIREBALL_DEFAULT_FRAME {0.0f, 0.0f, 5.0f, 5.0f}
#define PLAYER_CAST_POSITION {38.0f, 18.0f}
#define FIREBALL_COOLDOWN 0.4f
#define FIREBALL_SPEED 300.0f
#define STARBALL_DEFAULT_SPEED 150.0f

void initFireballs(Game *game) {
    game->fireballs.deathCooldown = 0.0f;
    initCollisionMap(FIREBALL_IMAGE_PATH, (Rectangle)FIREBALL_DEFAULT_FRAME, 10, &game->fireballs.collisions);
}

void createFireball(Position *position, Velocity *velocity, EcsEntityHandle caster, float damage, Game *game) {
    EcsEntityHandle handle = ecsCreateEntity();
    addComponent(handle, position);
    addComponent(handle, velocity);

    Spell spell = {.caster = caster, .damage = damage};
    addComponent(handle, &spell);

    Fireball fireball = {.caster = caster};
    addComponent(handle, &fireball);

    Render rndr = {&game->resources.sprites[SPRITESHEET_FIREBALL], FIREBALL_DEFAULT_FRAME};
    addComponent(handle, &rndr);

    EntityState state = {.id = ENTITY_STATE_IDLE, .cooldown = 0.0f};
    addComponent(handle, &state);

    eventCreate(handle, EVENT_FIREBALL_CAST, game);
}

void createIceball(EcsEntityHandle casterHandle, float damage, float angle, Game *game) {
    EcsEntityHandle handle = ecsCreateEntity();
    OrbitMovement orbit = {.center = casterHandle, .radius = 60.0, .angle = angle, .angularSpeed = 2.0f};
    addComponent(handle, &orbit);

    Position *casterPosition = getPosition(casterHandle);
    Rectangle casterFrame = getRender(casterHandle)->frame;

    Position position {
        .x = casterPosition->x + casterFrame.width / 2.0f,
        .y = casterPosition->y + casterFrame.height / 2.0f - orbit.radius};
    addComponent(handle, &position);

    Spell spell = {.caster = casterHandle, .type = COMPONENT_ID(Iceball), .damage = damage};
    addComponent(handle, &spell);

    Iceball iceball = {.max = 2};
    addComponent(handle, &iceball);

    EntityState entityState = {.id = ENTITY_STATE_IDLE, .cooldown = 0.0f};
    addComponent(handle, &entityState);

    Render render = {.spritesheet = &game->resources.sprites[SPRITESHEET_ICEBALL], .frame = (Rectangle){0, 0, 9, 9}};
    addComponent(handle, &render);

    // TODO: Add animation for iceball
}

EcsEntityHandle findNearest(Position position, EcsComponentId componentId) {
    EcsEntityHandle nearest = INVALID_HANDLE;
    float minDistance = 999999.9f;

    EcsComponentId componentIdList[] = {componentId};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 1);
    EcsEntityHandle handle;
    while((handle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Position *enemyPosition = getPosition(handle);
        Rectangle enemyFrame = getRender(handle)->frame;

        Position centeredEnemyPosition =
            {.x = enemyPosition->x + enemyFrame.x / 2.0f, .y = enemyPosition->y + enemyFrame.y / 2.0f};

        float distance = xyMagnitude(centeredEnemyPosition.x - position.x, centeredEnemyPosition.y - position.y);
        if (distance < minDistance) {
            nearest = handle;
            minDistance = distance;
        }
    }

    return nearest;
}


void createStarball(Position position, EcsEntityHandle casterHandle, float damage, Game *game) {
    EcsEntityHandle target = findNearest(position, COMPONENT_ID(Enemy));
    if (target == INVALID_HANDLE) {
        return;
    }

    EcsEntityHandle starballHandle = ecsCreateEntity();
    addComponent(starballHandle, &position);

    Velocity velocity  = {0};
    addComponent(starballHandle, &velocity);

    Spell spell = {.caster = casterHandle, .type = COMPONENT_ID(Homing), .damage = damage};
    addComponent(starballHandle, &spell);

    EntityState entityState = {.id = ENTITY_STATE_IDLE, .cooldown = 0.0f};
    addComponent(starballHandle, &entityState);

    Homing homing = {.target = target, .speed = STARBALL_DEFAULT_SPEED};
    addComponent(starballHandle, &homing);

    Render render = {.spritesheet = &game->resources.sprites[SPRITESHEET_STARBALL], .frame = Rectangle{0, 0, 9, 9}};
    addComponent(starballHandle, &render);
}

void systemHomingVelocityUpdate() {
    EcsComponentId componentIdList[] = {COMPONENT_ID(Homing)};

    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 1);
    EcsEntityHandle handle;
    while((handle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        if (getEntityState(handle)->id != ENTITY_STATE_IDLE) {
            continue;
        }


        Homing *homing= getHoming(handle);
        Position *homingPosition = getPosition(handle);
        Velocity *velocity = getVelocity(handle);
        if (!ecsDoesEntityExist(homing->target)) {
            homing->target = findNearest(*homingPosition, COMPONENT_ID(Enemy));
            if (homing->target == INVALID_HANDLE) {
                *velocity = {0.0f, 0.0f};

                continue;
            }
        }

        Position *targetPosition = getPosition(getHoming(handle)->target);

        Vector2 direction = xyNormalize(targetPosition->x - homingPosition->x, targetPosition->y - homingPosition->y);

        *velocity = toVelocity(vector2Scale(direction, getHoming(handle)->speed));
    }
}

void castPlayerSpells(Game *game, float dt) {
    // if (game->timer < 5.0f) {
    //     return;
    // }

    Position *playerPosition = getPosition(game->player.handle);
    PlayerWeaponList *weaponList = getPlayerWeaponList(game->player.handle);
    WeaponSlot *weaponSlot;
    for (int i = 0; i < 5; i++) {
        weaponSlot = &weaponList->weapons[i];
        Position weaponPosition = toPosition(
            vector2Sum(
                toVector2(*playerPosition),
                (Vector2)PLAYER_CAST_POSITION
            )
        );
        if (weaponSlot->type == PLAYER_WEAPON_FIREBALL) {
            if (weaponSlot->cooldown <= 0) {
                Velocity fireballVelocity = {FIREBALL_SPEED, 0};

                createFireball(&weaponPosition, &fireballVelocity, game->player.handle, weaponSlot->damage, game);
                weaponSlot->cooldown = PLAYER_FIREBALL_COOLDOWN;
            }

            weaponSlot->cooldown -= dt;

            continue;
        }

        if (weaponSlot->type == PLAYER_WEAPON_ICEBALL) {
            if (ecsGetComponentCount(COMPONENT_ID(Iceball)) > 1) {
                continue;
            }

            // TODO: scale up for any count of iceballs
            EcsComponentId componentIdList[] = {COMPONENT_ID(Iceball)};
            EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 1);
            EcsEntityHandle iceballHandle = ecsGetNextEntity(&iterator);

            // TODO: add cooldown after iceball destroyed
            float angle = iceballHandle != INVALID_HANDLE ? getOrbitMovement(iceballHandle)->angle + PI : 0.0f;
            createIceball(game->player_handle, weaponSlot->damage, angle, game);

            continue;
        }

        if (weaponSlot->type == PLAYER_WEAPON_STARBALL) {
            if (weaponSlot->cooldown <= 0) {
                if (ecsGetComponentCount(COMPONENT_ID(Enemy)) == 0) {
                    continue;
                }
                createStarball(weaponPosition, game->player_handle, weaponSlot->damage, game);

                weaponSlot->cooldown = PLAYER_STARBALL_COOLDOWN;
            }

            weaponSlot->cooldown -= dt;

            continue;
        }
    }
}

void systemCollideSpells(Game *game) {
    EcsComponentId enemyComponentIdList[] = {COMPONENT_ID(Enemy), COMPONENT_ID(Position), COMPONENT_ID(Render)};
    EcsEntityIterator enemyIterator = ecsGetEntityIterator(enemyComponentIdList, 3);

    EcsComponentId fireballComponentIdList[] = {COMPONENT_ID(Spell), COMPONENT_ID(Position), COMPONENT_ID(Render)};
    EcsEntityIterator fireballIterator = ecsGetEntityIterator(fireballComponentIdList, 3);

    EcsEntityHandle fireballHandle;
    EcsEntityHandle enemyHandle;
    while ((fireballHandle = ecsGetNextEntity(&fireballIterator)) != INVALID_HANDLE) {
        EntityState *fireballState =
            (EntityState *)ecsGetEntityComponent(game->components[CMP_ENTITY_STATE], fireballHandle);
        if (fireballState->id != ENTITY_STATE_IDLE) {
            continue;
        }

        Spell *fireball = getSpell(fireballHandle);

        Position *fireballPosition = getPosition(fireballHandle);
        Render *fireballRender = getRender(fireballHandle);
        Animation *fireballAnim = getAnimation(fireballHandle);

        uint32_t fireballFrameIndex = fireballAnim ? fireballAnim->currentFrame : 0;

        if (fireball->caster != game->player.handle) {
            Animation *playerAnim = getAnimation(game->player.handle);
            Position *playerPos = getPosition(game->player.handle);
            if(checkCollision(
                &game->fireballs.collisions, fireballFrameIndex, fireballPosition->x, fireballPosition->y,
                &game->player.collisions, playerAnim->currentFrame, playerPos->x, playerPos->y
            )) {
                eventCreate(fireballHandle, EVENT_FIREBALL_HIT, game);
                damage(game->player.handle, fireball->damage, game);
            }

            continue;
        }

        EcsEntityIterator enemyIterator = ecsGetEntityIterator(enemyComponentIdList, 3);
        while ((enemyHandle = ecsGetNextEntity(&enemyIterator)) != INVALID_HANDLE) {
            EntityState *enemyState = getEntityState(enemyHandle);
            if (enemyState->id != ENTITY_STATE_IDLE) {
                continue;
            }

            Position *enemyPosition = getPosition(enemyHandle);
            Render *enemyRender = getRender(enemyHandle);
            Animation *enemyAnim = getAnimation(enemyHandle);

            uint32_t enemyFrameIndex = 0;
            if (enemyAnim) {
                enemyFrameIndex = enemyAnim->currentFrame;
            }

            if (checkCollision(
                &game->fireballs.collisions, fireballFrameIndex, fireballPosition->x, fireballPosition->y,
                &game->enemies.collisions, enemyFrameIndex, enemyPosition->x, enemyPosition->y
            )) {
                eventCreate(fireballHandle, EVENT_FIREBALL_HIT, game);
                damage(enemyHandle, fireball->damage, game);

                break;
            }
        }
    }
}

void systemCleanFireballs() {
    EcsComponentId componentIdList[] = {COMPONENT_ID(Spell), COMPONENT_ID(Position)};
    EcsEntityIterator iterator = ecsGetEntityIterator(componentIdList, 2);
    EcsEntityHandle entityHandle;
    while ((entityHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        EntityState *fireballState = getEntityState(entityHandle);
        if (fireballState->id == ENTITY_STATE_DIE) {
            continue;
        }

        Position *position = getPosition(entityHandle);

        if (position->x > PLAYSCREEN_WIDTH || position->x < -(Rectangle)FIREBALL_DEFAULT_FRAME.width
            || position->y > PLAYSCREEN_HEIGHT || position->y < -(Rectangle)FIREBALL_DEFAULT_FRAME.height
        ) {
            fireballState->id = ENTITY_STATE_DIE;
        }
    }
}
