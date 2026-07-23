#include "smetanka_ecs.h"
#include "witch_event.h"
#include "witch_animation.h"
#include "witch_components.h"
#include "witch_game.h"

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
        EntityState *entityState = getEntityState(event->entityHandle);

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
                // In idle state, it is used to show damage flash effect on the entity
                entityState->cooldown = 0.15f;

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

                break;
            }
        }

        ecsAddToDestroyQueue(eventHandle);
    }

    ecsFlushDestroyQueue();
}
