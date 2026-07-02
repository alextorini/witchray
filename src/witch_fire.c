#include "witch_fire.h"
#include "ext/raylib.h"
#include "ext/raymath.h"
#include "smetanka_ecs.h"
#include "witch_components.h"
#include "witch_game.h"
#include "witch_resources.h"

#define FIREBALL_DEFAULT_FRAME {0.0f, 0.0f, 3.0f, 3.0f}
#define FIREBALL_CAST_POSITION {38.0f, 18.0f}
#define FIREBALL_COOLDOWN 0.4
#define FIREBALL_SPEED 300.0

static float fireball_cooldown = 0.0;

void init_fireballs() {
    init_collision_map(FIREBALL_IMAGE_PATH, (Rectangle)FIREBALL_DEFAULT_FRAME, 10, &game.fireballs.collisions);
}

static void create_fireball(Position *position, Velocity *velocity) {
    EcsEntityHandle handle = ecs_create_entity();
    ecs_add_component(handle, game.components[CMP_POSITION], position);
    ecs_add_component(handle, game.components[CMP_VELOCITY], velocity);

    Fireball fireball = {1};
    ecs_add_component(handle, game.components[CMP_FIREBALL], &fireball);

    Render rndr = {&game.resources.sprites[SPRITESHEET_FIREBALL], FIREBALL_DEFAULT_FRAME};
    ecs_add_component(handle, game.components[CMP_RENDER], &rndr);
}

void cast_fireballs(EcsEntityHandle player_handle, float delta_time) {
    Position *player_position = (Position *)ecs_get_entity_component(game.components[CMP_POSITION], player_handle);
    if (fireball_cooldown <= 0) {
        Position fireball_position = Vector2Add(*player_position, (Position)FIREBALL_CAST_POSITION);
        Velocity fireball_velocity = {FIREBALL_SPEED, 0};
        create_fireball(&fireball_position, &fireball_velocity);
        fireball_cooldown = FIREBALL_COOLDOWN;

        return;
    }

    fireball_cooldown -= delta_time;
}

void system_fireballs_collide_enemies(
    EcsComponentId fireball_id,
    EcsComponentId enemy_id,
    EcsComponentId position_id,
    EcsComponentId render_id
) {
    EcsComponentId enemy_component_id_list[] = {enemy_id, position_id, render_id};
    EcsEntityIterator enemy_iterator = ecs_get_entity_iterator(enemy_component_id_list, 3);

    EcsComponentId fireball_component_id_list[] = {fireball_id, position_id, render_id};
    EcsEntityIterator fireball_iterator = ecs_get_entity_iterator(fireball_component_id_list, 3);

    EcsEntityHandle fireball_handle;
    EcsEntityHandle enemy_handle;
    while ((fireball_handle = ecs_get_next_entity(&fireball_iterator)) != INVALID_HANDLE) {
        Position *fireball_position = (Position *)ecs_get_entity_component(position_id, fireball_handle);
        Render *fireball_render = (Render *)ecs_get_entity_component(render_id, fireball_handle);
        Animation *fireball_anim =
            (Animation *)ecs_get_entity_component(game.components[CMP_ANIMATION], fireball_handle);
        uint32_t fireball_frame_index = 0;
        if (fireball_anim) fireball_frame_index = fireball_anim->current_frame;
        EcsEntityIterator enemy_iterator = ecs_get_entity_iterator(enemy_component_id_list, 3);
        while ((enemy_handle = ecs_get_next_entity(&enemy_iterator)) != INVALID_HANDLE) {
            Position *enemy_position = (Position *)ecs_get_entity_component(position_id, enemy_handle);
            Render *enemy_render = (Render *)ecs_get_entity_component(render_id, enemy_handle);
            Animation *enemy_anim = (Animation *)ecs_get_entity_component(game.components[CMP_ANIMATION], enemy_handle);

            uint32_t enemy_frame_index = 0;
            if (enemy_anim) {
                enemy_frame_index = enemy_anim->current_frame;
            }

            if (pixel_perfect_collision(
                &game.fireballs.collisions, fireball_frame_index, fireball_position->x, fireball_position->y,
                &game.enemies.collisions, enemy_frame_index, enemy_position->x, enemy_position->y
            )) {
                ecs_destroy_entity(enemy_handle);
                ecs_destroy_entity(fireball_handle);
                game.enemies_killed++;
            }
        }
    }
}
