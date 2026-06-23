#include <stdint.h>
#include <stdlib.h>
#include "smetanka_ecs.h"
#include "witch_core.h"

#define SPAWN_COOLDOWN 0.5
#define ENEMY_SPEED 100.0
#define MAX_ENEMIES_COUNT 10000
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32

#define ENEMY_FRAME_TIME 0.5

static uint16_t enemies_count;
static float spawn_cooldown;

static AnimationClip idle_animation;
static AnimationSet animation_set;

void init_enemy_factory() {
    enemies_count = 0;
    spawn_cooldown = SPAWN_COOLDOWN;

    idle_animation.start_frame = 0;
    idle_animation.end_frame = 1;
    idle_animation.frame_time = ENEMY_FRAME_TIME;
    idle_animation.loop = 1;
    animation_set.clips = WR_MALLOC_TYPE(AnimationClip);
    animation_set.count = 1;
    animation_set.clips[0] = idle_animation;
}

EcsEntityHandle create_enemy(Position *pos) {
    EcsEntityHandle enemy_handle = ecs_create_entity();

    ecs_add_component(enemy_handle, ecs_hndls.cmpnts.pos, pos);
    Render rndr = {&sprtshts.enemy, ENEMY_DEFAULT_FRAME};
    ecs_add_component(enemy_handle, ecs_hndls.cmpnts.rndr, &rndr);

    Velocity vel = {-ENEMY_SPEED, 0};
    ecs_add_component(enemy_handle, ecs_hndls.cmpnts.vel, &vel);

    IsEnemy enmy = true;
    ecs_add_component(enemy_handle, ecs_hndls.cmpnts.enmy, &enmy);

    Animation animation;
    animation.set = &animation_set;
    animation.current_clip = 0;
    animation.current_frame = 0;
    animation.timer = 0.0;
    ecs_add_component(enemy_handle, ecs_hndls.cmpnts.anim, &animation);

    enemies_count++;

    return enemy_handle;
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

void system_clean_enemies() {
    EcsComponentId pos_cmp_id = ecs_hndls.cmpnts.pos;
    EcsComponentId enmy_cmp_id = ecs_hndls.cmpnts.enmy;

    uint32_t enemies_count = ecs_get_component_count(enmy_cmp_id);
    for (uint32_t i = 0; i < enemies_count ; i++) {
        EcsEntityHandle ent_id = ecs_get_component_dense(enmy_cmp_id, i);
        if (ent_id == INVALID_ID) {
            continue;
        }

        Position *ent_pos = (Position *)ecs_get_entity_component(pos_cmp_id, ent_id);
        if (!ent_pos) {
            continue;
        }

        if (ent_pos->x < -ENEMY_WIDTH) {
            ecs_destroy_entity(ent_id);
        }
    }
}
