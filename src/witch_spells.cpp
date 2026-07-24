#include "witch_spells.h"
#include "ext/raylib.h"
#include "smetanka_ecs.h"
#include "witch_collisions.h"
#include "witch_components.h"
#include "witch_damage.h"
#include "witch_event.h"
#include "witch_game.h"
#include "witch_resources.h"

#define FIREBALL_DEFAULT_FRAME {0.0f, 0.0f, 5.0f, 5.0f}
#define FIREBALL_CAST_POSITION {38.0f, 18.0f}
#define FIREBALL_COOLDOWN 0.4f
#define FIREBALL_SPEED 300.0f

void initFireballs(Game *game) {
    game->fireballs.deathCooldown = 0.0f;
    initCollisionMap(FIREBALL_IMAGE_PATH, (Rectangle)FIREBALL_DEFAULT_FRAME, 10, &game->fireballs.collisions);
}

void createFireball(Position *position, Velocity *velocity, EcsEntityHandle caster, Game *game) {
    EcsEntityHandle handle = ecsCreateEntity();
    addComponent(handle, position);
    addComponent(handle, velocity);

    Spell fireball = {.caster = caster, .damage = 1};
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

    Spell spell = {.caster = casterHandle, .damage = damage};
    addComponent(handle, &spell);

    // This Iceball component is temporary solution
    // TODO: Need to find scalable solution to distinguish between spells
    Iceball iceball = {.caster = casterHandle, . damage = damage};
    addComponent(handle, &iceball);

    EntityState entityState = {.id = ENTITY_STATE_IDLE, .cooldown = 0.0f};
    addComponent(handle, &entityState);

    Render render = {.spritesheet = &game->resources.sprites[SPRITESHEET_ICEBALL], .frame = (Rectangle){0, 0, 9, 9}};
    addComponent(handle, &render);

    // TODO: Add animation for iceball
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
        if (weaponSlot->type == PLAYER_WEAPON_FIREBALL) {
            if (weaponSlot->cooldown <= 0) {
                Position fireballPosition = toPosition(
                    vector2Sum(
                        toVector2(*playerPosition),
                        (Vector2)FIREBALL_CAST_POSITION
                    )
                );
                Velocity fireballVelocity = {FIREBALL_SPEED, 0};

                createFireball(&fireballPosition, &fireballVelocity, game->player_handle, game);
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

        Spell *fireball = getFireball(fireballHandle);

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
