#pragma once

#include "smetanka_ecs.h"
#include "witch_game.h"

typedef enum {
    EVENT_FIREBALL_CAST,
    EVENT_FIREBALL_HIT,
    EVENT_DAMAGE_TAKEN,
    EVENT_DEATH,
} EventType;

typedef struct {
    EventType type;
    EcsEntityHandle entityHandle;
} Event;

void eventCreate(EcsEntityHandle entityHandle, EventType eventType, Game *game);

void eventProcess(Game *game);
