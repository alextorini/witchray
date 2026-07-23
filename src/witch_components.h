#pragma once

#include <stdint.h>
#include "smetanka_ecs.h"
#include "smetanka_engine.h"
#include "witch_game.h"
#include "witch_event.h"

typedef Vector2 Position;
typedef Vector2 Velocity;
typedef uint8_t IsParallax;
typedef uint8_t IsEnemy;

typedef enum {
    CASTER_PLAYER,
    CASTER_ENEMY,
} Caster;

typedef struct {
    int32_t damage;
    Caster caster;
} Fireball;

typedef struct {
    Position center;
    float radius;
    float angle;
    float angularSpeed;
} OrbitMovement;

typedef struct {
    float current;
    float max;
} Health;

typedef struct {
    int16_t type;
    float damage;
    float cooldown;
} WeaponSlot;

typedef enum {
    ENTITY_STATE_IDLE,
    ENTITY_STATE_DYING,
    ENTITY_STATE_DIE,
    ENTITY_STATE_COUNT
} EntityStateIndex;

typedef struct {
    WeaponSlot weapons[3];
} EnemyWeaponList;

typedef struct {
    WeaponSlot weapons[5];
} PlayerWeaponList;

typedef struct {
    EntityStateIndex id;
    float cooldown;
} EntityState;

typedef struct {
    Texture *spritesheet;
    Rectangle frame;
    // int priority;
} Render;

typedef struct {
    uint16_t startFrame;
    uint16_t endFrame;
    float frameTime;
    uint8_t loop;
} AnimationClip;

typedef struct {
    AnimationClip clips[2];
} AnimationSet;

typedef struct {
    AnimationSet *set;
    uint16_t currentClip;
    uint16_t currentFrame;
    float timer;
} Animation;

typedef struct {
    Position offset;
    Color color;
    Font font;
    float fontSize;
    float spacing;
    char text[128];
} TextRender;

typedef enum {
    CMP_EVENT,
    CMP_POSITION,
    CMP_VELOCITY,
    CMP_ORBIT_MOVEMENT,
    CMP_RENDER,
    CMP_ANIMATION,
    CMP_PARALLAX,
    CMP_ENEMY,
    CMP_TEXT_RENDER,
    CMP_FIREBALL,
    CMP_ENEMY_WEAPON_LIST,
    CMP_PLAYER_WEAPON_LIST,
    CMP_ENTITY_STATE,
    CMP_HEALTH,
    CMP_COUNT
} ComponentIndex;

void initComponents(Game *game);

inline Animation *getEntityAnimation(EcsEntityHandle handle, Game *game) {
    return (Animation *)ecsGetEntityComponent(game->components[CMP_ANIMATION], handle);
}

inline IsEnemy *getEntityIsEnemy(EcsEntityHandle handle, Game *game) {
    return (IsEnemy *)ecsGetEntityComponent(game->components[CMP_ENEMY], handle);
}

inline Event *getEntityEvent(EcsEntityHandle handle, Game *game) {
    return (Event *)ecsGetEntityComponent(game->components[CMP_EVENT], handle);
}

inline Fireball *getEntityFireball(EcsEntityHandle handle, Game *game) {
    return (Fireball *)ecsGetEntityComponent(game->components[CMP_FIREBALL], handle);
}

inline IsParallax *getEntityIsParallax(EcsEntityHandle handle, Game *game) {
    return (IsParallax *)ecsGetEntityComponent(game->components[CMP_PARALLAX], handle);
}

inline Position *getEntityPosition(EcsEntityHandle handle, Game *game) {
    return (Position *)ecsGetEntityComponent(game->components[CMP_POSITION], handle);
}

inline Render *getEntityRender(EcsEntityHandle handle, Game *game) {
    return (Render *)ecsGetEntityComponent(game->components[CMP_RENDER], handle);
}

inline TextRender *getEntityTextRender(EcsEntityHandle handle, Game *game) {
    return (TextRender *)ecsGetEntityComponent(game->components[CMP_TEXT_RENDER], handle);
}

inline Velocity *getEntityVelocity(EcsEntityHandle handle, Game *game) {
    return (Velocity *)ecsGetEntityComponent(game->components[CMP_VELOCITY], handle);
}

inline OrbitMovement *getEntityOrbitMovement(EcsEntityHandle handle, Game *game) {
    return (OrbitMovement *)ecsGetEntityComponent(game->components[CMP_ORBIT_MOVEMENT], handle);
}

inline EnemyWeaponList *getEntityEnemyWeaponList(EcsEntityHandle handle, Game *game) {
    return (EnemyWeaponList *)ecsGetEntityComponent(game->components[CMP_ENEMY_WEAPON_LIST], handle);
}

inline PlayerWeaponList *getEntityPlayerWeaponList(EcsEntityHandle handle, Game *game) {
    return (PlayerWeaponList *)ecsGetEntityComponent(game->components[CMP_PLAYER_WEAPON_LIST], handle);
}

inline EntityState *getEntityState(EcsEntityHandle handle, Game *game) {
    return (EntityState *)ecsGetEntityComponent(game->components[CMP_ENTITY_STATE], handle);
}

inline Health *getEntityHealth(EcsEntityHandle handle, Game *game) {
    return (Health *)ecsGetEntityComponent(game->components[CMP_HEALTH], handle);
}
