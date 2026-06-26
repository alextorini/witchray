#include <stdint.h>
#include <stdlib.h>
#include "smetanka_ecs.h"
#include "smetanka_misc.h"
#include "witch_components.h"
#include "witch_core.h"
#include "witch_resources.h"

#define SPAWN_COOLDOWN 0.01
#define ENEMY_SPEED 100.0
#define MAX_ENEMIES_COUNT 10000
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32

#define ENEMY_FRAME_TIME 0.5

static uint16_t enemies_count;
static float spawn_cooldown;

// static AnimationClip idle_animation;
// static AnimationSet animation_set;

void init_enemy_factory() {
    enemies_count = 0;
    spawn_cooldown = SPAWN_COOLDOWN;

    // idle_animation.start_frame = 0;
    // idle_animation.end_frame = 1;
    // idle_animation.frame_time = ENEMY_FRAME_TIME;
    // idle_animation.loop = 1;
    // animation_set.clips = WR_MALLOC_TYPE(AnimationClip);
    // animation_set.count = 1;
    // animation_set.clips[0] = idle_animation;
}

EcsEntityHandle create_enemy(Position *pos) {
    EcsEntityHandle handle = ecs_create_entity();

    ecs_add_component(handle, game.components[CMP_POSITION], pos);
    Render rndr = {&game.resources.sprites[SPRITESHEET_ENEMY], ENEMY_DEFAULT_FRAME};
    ecs_add_component(handle, game.components[CMP_RENDER], &rndr);

    Velocity vel = {-ENEMY_SPEED, 0};
    ecs_add_component(handle, game.components[CMP_VELOCITY], &vel);

    IsEnemy enmy = true;
    ecs_add_component(handle, game.components[CMP_ENEMY], &enmy);

    // Animation animation;
    // animation.set = &animation_set;
    // animation.current_clip = 0;
    // animation.current_frame = 0;
    // animation.timer = 0.0;
    // ecs_add_component(handle, game.components[CMP_ANIMATION], &animation);
    //
    enemies_count++;

    return handle;
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

void system_clean_enemies(EcsComponentId enemy_id, EcsComponentId position_id) {
    EcsComponentId component_id_list[] = {enemy_id, position_id};
    EcsEntityIterator iterator = ecs_get_entity_iterator(component_id_list, 2);
    EcsEntityHandle entity_handle;
    while ((entity_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *position = (Position *)ecs_get_entity_component(position_id, entity_handle);

        if (position->x < -ENEMY_WIDTH) {
            ecs_destroy_entity(entity_handle);
        }
    }
}

void system_collide_enemies(
    EcsEntityHandle player_handle,
    EcsComponentId enemy_id,
    EcsComponentId position_id,
    EcsComponentId render_id
) {
    EcsComponentId component_id_list[] = {enemy_id, position_id, render_id};
    EcsEntityIterator iterator = ecs_get_entity_iterator(component_id_list, 3);

    Position *player_position = (Position *)ecs_get_entity_component(game.components[CMP_POSITION], player_handle);
    Render *player_render = (Render *)ecs_get_entity_component(game.components[CMP_RENDER], player_handle);

    EcsEntityHandle enemy_handle;
    while ((enemy_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *enemy_position = (Position *)ecs_get_entity_component(position_id, enemy_handle);
        Render *enemy_render = (Render *)ecs_get_entity_component(render_id, enemy_handle);

        if (aabb_intersect(
                player_position->x, player_position->y, player_render->frame.width, player_render->frame.height,
                enemy_position->x, enemy_position->y, enemy_render->frame.width, enemy_render->frame.height
            )) {
            should_close = 1;
        }
    }
}

