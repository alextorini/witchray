#include <stdint.h>

#include "witch_fire.h"
#include "witch_game.h"
#include "witch_player.h"
#include "witch_resources.h"

#define PLAYER_MAX_VELOCITY 200.0
#define PLAYER_ACCELERATION 500.0
#define PLAYER_BRAKING  250.0

static AnimationClip idle_animation;
static AnimationSet animation_set;

EcsEntityHandle init_player(Game *game) {
    EcsEntityHandle handle = ecs_create_entity();

    Position position = PLAYER_START_POS;
    ecs_add_component(handle, game->components[CMP_POSITION], &position);

    Velocity velocity = {0.0, 0.0};
    ecs_add_component(handle, game->components[CMP_VELOCITY], &velocity);

    Render render = {&game->resources.sprites[SPRITESHEET_PLAYER], PLAYER_DEFAULT_FRAME};
    ecs_add_component(handle, game->components[CMP_RENDER], &render);

    idle_animation.start_frame = 0;
    idle_animation.end_frame = 1;
    idle_animation.frame_time = PLAYER_IDLE_ANIM_SPEED;
    idle_animation.loop = 1;

    animation_set.clips = WR_MALLOC_TYPE(AnimationClip);
    animation_set.count = 1;
    animation_set.clips[0] = idle_animation;

    Animation animation;
    animation.set = &animation_set;
    animation.current_clip = 0;
    animation.current_frame = 0;
    animation.timer = 0.0;
    ecs_add_component(handle, game->components[CMP_ANIMATION], &animation);

    init_collision_map(PLAYER_IMAGE_PATH, render.frame, 6, &game->player.collisions);
    init_fireballs(game);

    return handle;
}
