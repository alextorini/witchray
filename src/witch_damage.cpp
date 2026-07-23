#include "smetanka_ecs.h"
#include "witch_components.h"
#include "witch_event.h"
#include "witch_game.h"

void damage(EcsEntityHandle entityHandle, float damage, Game *game) {
    EntityState *state = getEntityState(entityHandle);
    if (!state || state->id == ENTITY_STATE_DIE || state->id == ENTITY_STATE_DYING) {
        return;
    }

    eventCreate(entityHandle, EVENT_DAMAGE_TAKEN, game);

    Health *health = getHealth(entityHandle);
    health->current -= damage;

    if (health->current <= 0) {
        eventCreate(entityHandle, EVENT_DEATH, game);
    }
}
