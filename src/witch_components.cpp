#include "witch_components.h"
#include "smetanka_ecs.h"
#include "witch_game.h"

static EcsComponentId *components;

void initComponents(Game *game) {
    components = WR_MALLOC_ARR(EcsComponentId, CMP_COUNT);
    components[CMP_ANIMATION] = ecsRegisterComponent((char *)"Animation", sizeof(Animation));
    components[CMP_ENEMY] = ecsRegisterComponent((char *)"Enemy", sizeof(Enemy));
    components[CMP_EVENT] = ecsRegisterComponent((char *)"Event", sizeof(Event));
    components[CMP_SPELL] = ecsRegisterComponent((char *)"Spell", sizeof(Spell));
    components[CMP_FIREBALL] = ecsRegisterComponent((char *)"Fireball", sizeof(Fireball));
    components[CMP_ICEBALL] = ecsRegisterComponent((char *)"Iceball", sizeof(Iceball));
    components[CMP_HOMING] = ecsRegisterComponent((char *)"Homing", sizeof(Homing));
    components[CMP_PARALLAX] = ecsRegisterComponent((char *)"Parallax", sizeof(Parallax));
    components[CMP_POSITION] = ecsRegisterComponent((char *)"Position", sizeof(Position));
    components[CMP_RENDER] = ecsRegisterComponent((char *)"Render", sizeof(Render));
    components[CMP_TEXT_RENDER] = ecsRegisterComponent((char *)"Text", sizeof(TextRender));
    components[CMP_VELOCITY] = ecsRegisterComponent((char *)"Velocity", sizeof(Velocity));
    components[CMP_ORBIT_MOVEMENT] = ecsRegisterComponent((char *)"OrbitMovement", sizeof(OrbitMovement));
    components[CMP_ENEMY_WEAPON_LIST] = ecsRegisterComponent((char *)"EnemyWeaponList", sizeof(EnemyWeaponList));
    components[CMP_PLAYER_WEAPON_LIST] = ecsRegisterComponent((char *)"PlayerWeaponList", sizeof(PlayerWeaponList));
    components[CMP_ENTITY_STATE] = ecsRegisterComponent((char *)"EntityState", sizeof(EntityState));
    components[CMP_HEALTH] = ecsRegisterComponent((char *)"Health", sizeof(Health));

    game->components = components;
}

Animation *getAnimation(EcsEntityHandle handle) {
    return (Animation *)ecsGetEntityComponent(components[CMP_ANIMATION], handle);
}

Enemy *getEnemy(EcsEntityHandle handle) {
    return (Enemy *)ecsGetEntityComponent(components[CMP_ENEMY], handle);
}

Event *getEvent(EcsEntityHandle handle) {
    return (Event *)ecsGetEntityComponent(components[CMP_EVENT], handle);
}

Spell *getSpell(EcsEntityHandle handle) {
    return (Spell *)ecsGetEntityComponent(components[CMP_SPELL], handle);
}

Fireball *getFireball(EcsEntityHandle handle) {
    return (Fireball *)ecsGetEntityComponent(components[CMP_FIREBALL], handle);
}

Iceball *getIceball(EcsEntityHandle handle) {
    return (Iceball *)ecsGetEntityComponent(components[CMP_ICEBALL], handle);
}

Homing *getHoming(EcsEntityHandle handle) {
    return (Homing *)ecsGetEntityComponent(components[CMP_HOMING], handle);
}

Parallax *getParallax(EcsEntityHandle handle) {
    return (Parallax *)ecsGetEntityComponent(components[CMP_PARALLAX], handle);
}

Position *getPosition(EcsEntityHandle handle) {
    return (Position *)ecsGetEntityComponent(components[CMP_POSITION], handle);
}

Render *getRender(EcsEntityHandle handle) {
    return (Render *)ecsGetEntityComponent(components[CMP_RENDER], handle);
}

TextRender *getTextRender(EcsEntityHandle handle) {
    return (TextRender *)ecsGetEntityComponent(components[CMP_TEXT_RENDER], handle);
}

Velocity *getVelocity(EcsEntityHandle handle) {
    return (Velocity *)ecsGetEntityComponent(components[CMP_VELOCITY], handle);
}

OrbitMovement *getOrbitMovement(EcsEntityHandle handle) {
    return (OrbitMovement *)ecsGetEntityComponent(components[CMP_ORBIT_MOVEMENT], handle);
}

EnemyWeaponList *getEnemyWeaponList(EcsEntityHandle handle) {
    return (EnemyWeaponList *)ecsGetEntityComponent(components[CMP_ENEMY_WEAPON_LIST], handle);
}

PlayerWeaponList *getPlayerWeaponList(EcsEntityHandle handle) {
    return (PlayerWeaponList *)ecsGetEntityComponent(components[CMP_PLAYER_WEAPON_LIST], handle);
}

EntityState *getEntityState(EcsEntityHandle handle) {
    return (EntityState *)ecsGetEntityComponent(components[CMP_ENTITY_STATE], handle);
}

Health *getHealth(EcsEntityHandle handle) {
    return (Health *)ecsGetEntityComponent(components[CMP_HEALTH], handle);
}

void *addComponent(EcsEntityHandle handle, Animation *animation) {
    return ecsAddComponent(handle, components[CMP_ANIMATION], animation);
}

void *addComponent(EcsEntityHandle handle, Enemy *isEnemy) {
    return ecsAddComponent(handle, components[CMP_ENEMY], isEnemy);
}

void *addComponent(EcsEntityHandle handle, Event *event) {
    return ecsAddComponent(handle, components[CMP_EVENT], event);
}

void *addComponent(EcsEntityHandle handle, Spell *spell) {
    return ecsAddComponent(handle, components[CMP_SPELL], spell);
}

void *addComponent(EcsEntityHandle handle, Fireball *fireball) {
    return ecsAddComponent(handle, components[CMP_FIREBALL], fireball);
}

void *addComponent(EcsEntityHandle handle, Iceball *iceball) {
    return ecsAddComponent(handle, components[CMP_ICEBALL], iceball);
}

void *addComponent(EcsEntityHandle handle, Homing *starball) {
    return ecsAddComponent(handle, components[CMP_HOMING], starball);
}

void *addComponent(EcsEntityHandle handle, Parallax *isParallax) {
    return ecsAddComponent(handle, components[CMP_PARALLAX], isParallax);
}

void *addComponent(EcsEntityHandle handle, Position *position) {
    return ecsAddComponent(handle, components[CMP_POSITION], position);
}

void *addComponent(EcsEntityHandle handle, Render *render) {
    return ecsAddComponent(handle, components[CMP_RENDER], render);
}

void *addComponent(EcsEntityHandle handle, TextRender *textRender) {
    return ecsAddComponent(handle, components[CMP_TEXT_RENDER], textRender);
}

void *addComponent(EcsEntityHandle handle, Velocity *velocity) {
    return ecsAddComponent(handle, components[CMP_VELOCITY], velocity);
}

void *addComponent(EcsEntityHandle handle, OrbitMovement *orbitMovement) {
    return ecsAddComponent(handle, components[CMP_ORBIT_MOVEMENT], orbitMovement);
}

void *addComponent(EcsEntityHandle handle, EnemyWeaponList *enemyWeaponList) {
    return ecsAddComponent(handle, components[CMP_ENEMY_WEAPON_LIST], enemyWeaponList);
}

void *addComponent(EcsEntityHandle handle, PlayerWeaponList *playerWeaponList) {
    return ecsAddComponent(handle, components[CMP_PLAYER_WEAPON_LIST], playerWeaponList);
}

void *addComponent(EcsEntityHandle handle, EntityState *entityState) {
    return ecsAddComponent(handle, components[CMP_ENTITY_STATE], entityState);
}

void *addComponent(EcsEntityHandle handle, Health *health) {
    return ecsAddComponent(handle, components[CMP_HEALTH], health);
}

template<typename T>
EcsComponentId getComponentId();

template<> EcsComponentId getComponentId<Animation>() {
    return components[CMP_ANIMATION];
}

template<> EcsComponentId getComponentId<Enemy>() {
    return components[CMP_ENEMY];
}

template<> EcsComponentId getComponentId<Event>() {
    return components[CMP_EVENT];
}

template<> EcsComponentId getComponentId<Spell>() {
    return components[CMP_SPELL];
}

template<> EcsComponentId getComponentId<Fireball>() {
    return components[CMP_FIREBALL];
}

template<> EcsComponentId getComponentId<Iceball>() {
    return components[CMP_ICEBALL];
}

template<> EcsComponentId getComponentId<Homing>() {
    return components[CMP_HOMING];
}

template<> EcsComponentId getComponentId<Parallax>() {
    return components[CMP_PARALLAX];
}

template<> EcsComponentId getComponentId<Position>() {
    return components[CMP_POSITION];
}

template<> EcsComponentId getComponentId<Render>() {
    return components[CMP_RENDER];
}

template<> EcsComponentId getComponentId<TextRender>() {
    return components[CMP_TEXT_RENDER];
}

template<> EcsComponentId getComponentId<Velocity>() {
    return components[CMP_VELOCITY];
}

template<> EcsComponentId getComponentId<OrbitMovement>() {
    return components[CMP_ORBIT_MOVEMENT];
}

template<> EcsComponentId getComponentId<EnemyWeaponList>() {
    return components[CMP_ENEMY_WEAPON_LIST];
}

template<> EcsComponentId getComponentId<PlayerWeaponList>() {
    return components[CMP_PLAYER_WEAPON_LIST];
}

template<> EcsComponentId getComponentId<EntityState>() {
    return components[CMP_ENTITY_STATE];
}

template<> EcsComponentId getComponentId<Health>() {
    return components[CMP_HEALTH];
}
