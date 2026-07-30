#pragma once

#include "smetanka_ecs.h"
#include "witch_game.h"

typedef enum {
    EVENT_FIREBALL_CAST,
    EVENT_FIREBALL_HIT,
    EVENT_DAMAGE_TAKEN,
    EVENT_PICKUP_EXP,
    EVENT_PICKUP_HP,
    EVENT_LEVEL_UP,
    EVENT_DEATH,
    EVENT_WIN,
    EVENT_PAUSE_TOGGLE,
} EventType;

typedef struct {
    EventType type;
    EcsEntityHandle entityHandle;
} Event;

void eventCreate(EcsEntityHandle entityHandle, EventType eventType, Game *game);

void eventProcess(Game *game);
