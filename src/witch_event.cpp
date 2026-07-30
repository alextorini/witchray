#include "smetanka_ecs.h"
#include "witch_event.h"
#include "smetanka_engine.h"
#include "witch_animation.h"
#include "witch_components.h"
#include "witch_game.h"
#include "witch_win.h"

void eventCreate(EcsEntityHandle entityHandle, EventType eventType, Game *game) {
    EcsEntityHandle eventHandle = ecsCreateEntity();

    Event event = {.type = eventType, .entityHandle = entityHandle};
    addComponent(eventHandle, &event);
}

void eventProcess(Game *game) {
    EcsComponentId eventId = COMPONENT_ID(Event);
    EcsComponentId entityStateId = COMPONENT_ID(EntityState);

    EcsEntityIterator iterator = ecsGetEntityIterator(&eventId, 1);
    EcsEntityHandle eventHandle;
    while ((eventHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Event *event = getEvent(eventHandle);
        EntityState *entityState = ecsDoesEntityExist(event->entityHandle) ? getEntityState(event->entityHandle) : NULL;

        switch (event->type) {
            case EVENT_PAUSE_TOGGLE: {
                smePlaySound(game->resources.sounds[SOUND_PAUSE]);

                break;
            }
            case EVENT_FIREBALL_CAST: {
                smeSetSoundVolume(game->resources.sounds[SOUND_SHOOT], 0.25f);
                smePlaySound(game->resources.sounds[SOUND_SHOOT]);

                break;
            }
            case EVENT_FIREBALL_HIT: {
                if (!entityState) {
                    break;
                }

                smeSetSoundVolume(game->resources.sounds[SOUND_EXPLOSION], 0.5f);
                smePlaySound(game->resources.sounds[SOUND_SHOOT]);

                entityState->id = ENTITY_STATE_DIE;

                break;
            }
            case EVENT_DAMAGE_TAKEN: {
                if (!entityState) {
                    break;
                }

                // This variable is used for different purposes in different entity states
                // In idle state, it's used to show a damage flash effect on the entity
                entityState->cooldown = 0.15f;

                break;
            }
            case EVENT_PICKUP_EXP: {
                smePlaySound(game->resources.sounds[SOUND_PICKUP_EXP]);

                break;
            }
            case EVENT_PICKUP_HP: {
                smePlaySound(game->resources.sounds[SOUND_PICKUP_HP]);

                break;
            }
            case EVENT_LEVEL_UP: {
                game->player.level++;
                game->player.exp -= game->player.max_exp;
                game->player.max_exp *= 1.1;

                Health *health = getHealth(game->player.handle);
                if (game->player.level == 2) {
                    PlayerWeaponList *weaponList = getPlayerWeaponList(game->player.handle);
                    weaponList->weapons[1] = {.type = PLAYER_WEAPON_ICEBALL, .damage = 2, .cooldown = PLAYER_ICEBALL_COOLDOWN};
                } else if (game->player.level == 3) {
                    PlayerWeaponList *weaponList = getPlayerWeaponList(game->player.handle);
                    weaponList->weapons[2] = {.type = PLAYER_WEAPON_STARBALL, .damage = 1, .cooldown = PLAYER_STARBALL_COOLDOWN};
                } else {
                    health->max += 1;
                }

                health->current = health->max;
                if (game->player.exp >= game->player.max_exp) {
                    eventCreate(game->player.handle, EVENT_LEVEL_UP, game);
                }

                break;
            }
            case EVENT_DEATH: {
                if (!entityState) {
                    break;
                }

                if (entityState->id == ENTITY_STATE_DIE || entityState->id == ENTITY_STATE_DYING) {
                    break;
                }

                entityState->id = ENTITY_STATE_DYING;

                Enemy *enemy = getEnemy(event->entityHandle);
                if (enemy) {
                    smePlaySound(game->resources.sounds[SOUND_EXPLOSION]);

                    entityState->cooldown = game->enemies.deathCooldown;

                    Animation *animation = getAnimation(event->entityHandle);
                    switchAnimation(animation, ANIMATION_DYING);

                    game->enemiesKilled++;

                    break;
                }

                if (event->entityHandle == game->player.handle) {
                    smePlaySound(game->resources.sounds[SOUND_PLAYER_DEATH]);

                    entityState->cooldown = game->player.deathCooldown;

                    Animation *animation = getAnimation(event->entityHandle);
                    switchAnimation(animation, ANIMATION_DYING);
                }
            } break;
            case EVENT_WIN: {
                win(game);
            } break;
        }

        ecsAddToDestroyQueue(eventHandle);
    }

    ecsFlushDestroyQueue();
}
