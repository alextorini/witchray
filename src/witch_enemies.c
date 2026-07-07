#include <stdint.h>
#include <stdlib.h>
#include "smetanka_ecs.h"
#include "witch_collisions.h"
#include "witch_components.h"
#include "witch_enemies.h"
#include "witch_game.h"
#include "witch_resources.h"
#include "witch_save.h"

#define SPAWN_COOLDOWN 0.3
#define ENEMY_SPEED 100.0
#define MAX_ENEMIES_COUNT 10000
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32

#define ENEMY_DEFAULT_FRAME {0.0f, 0.0f, 32.0f, 32.0f}
#define ENEMY_FRAME_TIME 0.5

#define ENM_MALLOC malloc
#define ENM_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define ENM_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define ENM_CALLOC calloc
#define ENM_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define ENM_FREE free

static uint16_t enemies_count;
static float spawn_cooldown;

static AnimationClip idle_animation;
static AnimationSet animation_set;

void init_enemy_factory(Game *game) {
    enemies_count = 0;
    spawn_cooldown = SPAWN_COOLDOWN;

    idle_animation.start_frame = 0;
    idle_animation.end_frame = 1;
    idle_animation.frame_time = ENEMY_FRAME_TIME;
    idle_animation.loop = 1;
    animation_set.clips = ENM_MALLOC_TYPE(AnimationClip);
    animation_set.count = 1;
    animation_set.clips[0] = idle_animation;

    init_collision_map(ENEMY_IMAGE_PATH, (Rectangle)ENEMY_DEFAULT_FRAME, 10, &game->enemies.collisions);
}

static EcsEntityHandle create_enemy(Position *pos, Game *game) {
    EcsEntityHandle handle = ecs_create_entity();

    ecs_add_component(handle, game->components[CMP_POSITION], pos);
    Render rndr = {&game->resources.sprites[SPRITESHEET_ENEMY], ENEMY_DEFAULT_FRAME};
    ecs_add_component(handle, game->components[CMP_RENDER], &rndr);

    Velocity vel = {-ENEMY_SPEED, 0};
    ecs_add_component(handle, game->components[CMP_VELOCITY], &vel);

    IsEnemy enmy = true;
    ecs_add_component(handle, game->components[CMP_ENEMY], &enmy);

    Animation animation;
    animation.set = &animation_set;
    animation.current_clip = 0;
    animation.current_frame = 0;
    animation.timer = 0.0;
    ecs_add_component(handle, game->components[CMP_ANIMATION], &animation);

    enemies_count++;

    return handle;
}

void system_spawn_enemies(Game *game, float dt) {
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

    create_enemy(&pos, game);

    spawn_cooldown = SPAWN_COOLDOWN;
}

void system_clean_enemies(Game *game) {
    EcsComponentId enemy_id = game->components[CMP_ENEMY];
    EcsComponentId position_id =game->components[CMP_POSITION];

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

void system_collide_enemies(Game *game) {
    EcsEntityHandle player_handle = game->player.handle;
    EcsComponentId enemy_id = game->components[CMP_ENEMY];
    EcsComponentId position_id = game->components[CMP_POSITION];
    EcsComponentId render_id = game->components[CMP_RENDER];

    EcsComponentId component_id_list[] = {enemy_id, position_id, render_id};
    EcsEntityIterator iterator = ecs_get_entity_iterator(component_id_list, 3);

    Position *player_position = (Position *)ecs_get_entity_component(position_id, player_handle);
    Render *player_render = (Render *)ecs_get_entity_component(render_id, player_handle);
    Animation *player_anim = (Animation *)ecs_get_entity_component(game->components[CMP_ANIMATION], player_handle);
    uint32_t player_frame_index = 0;
    if (player_anim) player_frame_index = player_anim->current_frame;

    EcsEntityHandle enemy_handle;
    while ((enemy_handle = ecs_get_next_entity(&iterator)) != INVALID_HANDLE) {
        Position *enemy_position = (Position *)ecs_get_entity_component(position_id, enemy_handle);
        Render *enemy_render = (Render *)ecs_get_entity_component(render_id, enemy_handle);
        Animation *enemy_anim = (Animation *)ecs_get_entity_component(game->components[CMP_ANIMATION], enemy_handle);

        uint32_t enemy_frame_index = 0;
        if (enemy_anim) {
            enemy_frame_index = enemy_anim->current_frame;
        }

        if (check_collision(
            &game->player.collisions, player_frame_index, player_position->x, player_position->y,
            &game->enemies.collisions, enemy_frame_index, enemy_position->x, enemy_position->y
        )) {
            save_game(game);
            should_close = 1;

            return;
        }
    }
}

