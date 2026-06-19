#include <stdlib.h>
#include "smetanka_ecs.h"
#include "witch_core.h"

#define SPAWN_COOLDOWN 0.1
#define ENEMY_SPEED 50.0
#define MAX_ENEMIES_COUNT 1000
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32

static uint16_t enemies_count;
static float spawn_cooldown;

void init_enemy_factory() {
    enemies_count = 0;
    spawn_cooldown = SPAWN_COOLDOWN;
}

EcsEntityId create_enemy(Position *pos) {
    EcsEntityId enemy_id = ecs_create_entity();

    ecs_add_component(enemy_id, ecs_hndls.cmpnts.pos, pos);
    Render rndr = {&sprtshts.enemy, ENEMY_DEFAULT_FRAME};
    ecs_add_component(enemy_id, ecs_hndls.cmpnts.rndr, &rndr);

    Velocity vel = {-ENEMY_SPEED, 0};
    ecs_add_component(enemy_id, ecs_hndls.cmpnts.vel, &vel);

    enemies_count++;

    return enemy_id;
}

void system_spawn_enemies(float dt) {
    if (spawn_cooldown >= 0) {
        spawn_cooldown -= dt;

        return;
    }

    if (enemies_count >= MAX_ENEMIES_COUNT) {
        return;
    }

    Position pos;
    pos.x = VIRTUAL_WIDTH;
    pos.y = rand() % (VIRTUAL_HEIGHT - ENEMY_HEIGHT + 1);

    create_enemy(&pos);

    spawn_cooldown = SPAWN_COOLDOWN;
}

