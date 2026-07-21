#include "smetanka_ecs.h"
#include "witch_event.h"
#include "witch_game.h"

void eventCreate(EcsEntityHandle entityHandle, EventType eventType, Game *game) {
    EcsEntityHandle eventHandle = ecsCreateEntity();

    Event event = {.type = eventType, .entityHandle = entityHandle};
    ecsAddComponent(eventHandle, game->components[CMP_EVENT], &event);
}

void eventProcess(Game *game) {
    EcsComponentId eventId = game->components[CMP_EVENT];
    EcsComponentId entityStateId = game->components[CMP_ENTITY_STATE];

    EcsEntityIterator iterator = ecsGetEntityIterator(&eventId, 1);
    EcsEntityHandle eventHandle;
    while ((eventHandle = ecsGetNextEntity(&iterator)) != INVALID_HANDLE) {
        Event *event = (Event *)ecsGetEntityComponent(eventId, eventHandle);
        EntityState *entityState = (EntityState *)ecsGetEntityComponent(entityStateId, event->entityHandle);

        switch (event->type) {
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

                IsEnemy *isEnemy = (IsEnemy *)ecsGetEntityComponent(game->components[CMP_ENEMY], event->entityHandle);
                if (isEnemy) {
                    smePlaySound(game->resources.sounds[SOUND_EXPLOSION]);

                    entityState->cooldown = game->enemies.deathCooldown;

                    Animation *animation = (Animation *)ecsGetEntityComponent(game->components[CMP_ANIMATION], event->entityHandle);
                    animation->currentClip = 1;
                    animation->currentFrame = 5;
                    animation->timer = 0.0f;

                    game->enemiesKilled++;

                    break;
                }

                if (event->entityHandle == game->player.handle) {
                    smePlaySound(game->resources.sounds[SOUND_PLAYER_DEATH]);

                    entityState->cooldown = game->player.deathCooldown;

                    Animation *animation = (Animation *)ecsGetEntityComponent(game->components[CMP_ANIMATION], event->entityHandle);
                    animation->currentClip = 1;
                    animation->currentFrame = 4;
                    animation->timer = 0.0f;
                }

                break;
            }
        }

        ecsAddToDestroyQueue(eventHandle);
    }

    ecsFlushDestroyQueue();
}
