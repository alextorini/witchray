#include "witch_components.h"
#include "smetanka_ecs.h"
#include "witch_game.h"

void initComponents(Game *game) {
    game->components = WR_MALLOC_ARR(EcsComponentId, CMP_COUNT);
    game->components[CMP_ANIMATION] = ecsRegisterComponent((char *)"Animation", sizeof(Animation));
    game->components[CMP_ENEMY] = ecsRegisterComponent((char *)"Enemy", sizeof(IsEnemy));
    game->components[CMP_EVENT] = ecsRegisterComponent((char *)"Event", sizeof(Event));
    game->components[CMP_FIREBALL] = ecsRegisterComponent((char *)"Fireball", sizeof(Fireball));
    game->components[CMP_PARALLAX] = ecsRegisterComponent((char *)"Parallax", sizeof(IsParallax));
    game->components[CMP_POSITION] = ecsRegisterComponent((char *)"Position", sizeof(Position));
    game->components[CMP_RENDER] = ecsRegisterComponent((char *)"Render", sizeof(Render));
    game->components[CMP_TEXT_RENDER] = ecsRegisterComponent((char *)"Text", sizeof(TextRender));
    game->components[CMP_VELOCITY] = ecsRegisterComponent((char *)"Velocity", sizeof(Velocity));
    game->components[CMP_ORBIT_MOVEMENT] = ecsRegisterComponent((char *)"OrbitMovement", sizeof(OrbitMovement));
    game->components[CMP_ENEMY_WEAPON_LIST] = ecsRegisterComponent((char *)"EnemyWeaponList", sizeof(EnemyWeaponList));
    game->components[CMP_PLAYER_WEAPON_LIST] = ecsRegisterComponent((char *)"PlayerWeaponList", sizeof(PlayerWeaponList));
    game->components[CMP_ENTITY_STATE] = ecsRegisterComponent((char *)"EntityState", sizeof(EntityState));
    game->components[CMP_HEALTH] = ecsRegisterComponent((char *)"Health", sizeof(Health));
}
