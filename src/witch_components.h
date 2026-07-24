#pragma once

#include <stdint.h>
#include "ext/raylib.h"
#include "smetanka_ecs.h"
#include "smetanka_engine.h"
#include "witch_game.h"
#include "witch_event.h"

#define COMPONENT_ID(T) (getComponentId<T>())

typedef struct {
    float x;
    float y;
} Position;


typedef struct {
    float x;
    float y;
} Velocity;

inline Velocity toVelocity(Position pos) {
    return (Velocity){pos.x, pos.y};
}

inline Velocity toVelocity(Vector2 vec) {
    return (Velocity){vec.x, vec.y};
}

inline Position toPosition(Velocity vel) {
    return (Position){vel.x, vel.y};
}

inline Position toPosition(Vector2 vec) {
    return (Position){vec.x, vec.y};
}

inline Vector2 toVector2(Position pos) {
    return (Vector2){pos.x, pos.y};
}

inline Vector2 toVector2(Velocity vel) {
    return (Vector2){vel.x, vel.y};
}

typedef struct {
    uint8_t value;
} Parallax;

typedef struct {
    uint8_t value;
} Enemy;

typedef struct {
    EcsEntityHandle caster;
    float damage;
} Spell;

typedef struct {
    EcsEntityHandle caster;
    float damage;
} Iceball;

typedef struct {
    EcsEntityHandle center;
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
    CMP_ICEBALL,
    CMP_ENEMY_WEAPON_LIST,
    CMP_PLAYER_WEAPON_LIST,
    CMP_ENTITY_STATE,
    CMP_HEALTH,
    CMP_COUNT
} ComponentIndex;

void initComponents(Game *game);

Animation *getAnimation(EcsEntityHandle handle);
Enemy *getEnemy(EcsEntityHandle handle);
Event *getEvent(EcsEntityHandle handle);
Spell *getFireball(EcsEntityHandle handle);
Parallax *getParallax(EcsEntityHandle handle);
Position *getPosition(EcsEntityHandle handle);
Render *getRender(EcsEntityHandle handle);
TextRender *getTextRender(EcsEntityHandle handle);
Velocity *getVelocity(EcsEntityHandle handle);
OrbitMovement *getOrbitMovement(EcsEntityHandle handle);
EnemyWeaponList *getEnemyWeaponList(EcsEntityHandle handle);
PlayerWeaponList *getPlayerWeaponList(EcsEntityHandle handle);
EntityState *getEntityState(EcsEntityHandle handle);
Health *getHealth(EcsEntityHandle handle);

void *addComponent(EcsEntityHandle handle, Animation *animation);
void *addComponent(EcsEntityHandle handle, Enemy *isEnemy);
void *addComponent(EcsEntityHandle handle, Event *event);
void *addComponent(EcsEntityHandle handle, Spell *fireball);
void *addComponent(EcsEntityHandle handle, Iceball *iceball);
void *addComponent(EcsEntityHandle handle, Parallax *isParallax);
void *addComponent(EcsEntityHandle handle, Position *position);
void *addComponent(EcsEntityHandle handle, Render *render);
void *addComponent(EcsEntityHandle handle, TextRender *textRender);
void *addComponent(EcsEntityHandle handle, Velocity *velocity);
void *addComponent(EcsEntityHandle handle, OrbitMovement *orbitMovement);
void *addComponent(EcsEntityHandle handle, EnemyWeaponList *enemyWeaponList);
void *addComponent(EcsEntityHandle handle, PlayerWeaponList *playerWeaponList);
void *addComponent(EcsEntityHandle handle, EntityState *entityState);
void *addComponent(EcsEntityHandle handle, Health *health);

template<typename T>
EcsComponentId getComponentId();
template<> EcsComponentId getComponentId<Animation>();
template<> EcsComponentId getComponentId<Enemy>();
template<> EcsComponentId getComponentId<Event>();
template<> EcsComponentId getComponentId<Spell>();
template<> EcsComponentId getComponentId<Iceball>();
template<> EcsComponentId getComponentId<Parallax>();
template<> EcsComponentId getComponentId<Position>();
template<> EcsComponentId getComponentId<Render>();
template<> EcsComponentId getComponentId<TextRender>();
template<> EcsComponentId getComponentId<Velocity>();
template<> EcsComponentId getComponentId<OrbitMovement>();
template<> EcsComponentId getComponentId<EnemyWeaponList>();
template<> EcsComponentId getComponentId<PlayerWeaponList>();
template<> EcsComponentId getComponentId<EntityState>();
template<> EcsComponentId getComponentId<Health>();
