#include <math.h>
#include <stdint.h>

#include "ext/raylib.h"

#include "smetanka_ecs.h"
#include "witch_components.h"
#include "witch_core.h"
#include "witch_player.h"

#define PLAYER_MAX_VELOCITY 200.0
#define PLAYER_ACCELERATION 500.0
#define PLAYER_BRAKING  250.0

static AnimationClip idle_animation;
static AnimationSet animation_set;

EcsEntityHandle init_player() {
    EcsEntityHandle handle = ecs_create_entity();

    Position position = PLAYER_START_POS;
    ecs_add_component(handle, game.components[CMP_POSITION], &position);

    Velocity velocity = {0.0, 0.0};
    ecs_add_component(handle, game.components[CMP_VELOCITY], &velocity);

    Render render = {&game.resources.sprites[SPRITESHEET_PLAYER], PLAYER_DEFAULT_FRAME};
    ecs_add_component(handle, game.components[CMP_RENDER], &render);

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
    ecs_add_component(handle, game.components[CMP_ANIMATION], &animation);

    return handle;
}

void process_input(EcsEntityHandle player_handle, float delta_time) {
    Velocity *velocity = (Velocity *)ecs_get_entity_component(game.components[CMP_VELOCITY], player_handle);
    Position *position = (Position *)ecs_get_entity_component(game.components[CMP_POSITION], player_handle);
    Render *render = (Render *)ecs_get_entity_component(game.components[CMP_RENDER], game.player_handle);

    float dx = 0.0;
    float dy = 0.0;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) dx--;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dx++;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) dy--;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) dy++;

    if (IsGamepadAvailable(0)) {
        dx += GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        dy += GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

        if (dx <= GAMEPAD_DEADZONE && dx > -GAMEPAD_DEADZONE) dx = 0;
        if (dy <= GAMEPAD_DEADZONE && dy > -GAMEPAD_DEADZONE) dy = 0;
    }

    if (dx > 1.0) dx = 1.0;
    if (dx < -1.0) dx = -1.0;
    if (dy > 1.0) dy = 1.0;
    if (dy < -1.0) dy = -1.0;

    if (dx != 0 && dy != 0) {
        float vector_length = sqrtf(dx * dx + dy * dy);
        dx = dx / vector_length;
        dy = dy / vector_length;
    }

    float vector_length;
    if (dx != 0 || dy != 0) {
        velocity->x += dx * delta_time * PLAYER_ACCELERATION;
        velocity->y += dy * delta_time * PLAYER_ACCELERATION;

        vector_length = sqrtf(velocity->x * velocity->x + velocity->y * velocity->y);
        if (vector_length > PLAYER_MAX_VELOCITY) {
            float ratio = PLAYER_MAX_VELOCITY / vector_length;
            velocity->x *= ratio;
            velocity->y *= ratio;
        }
    } else {
        vector_length = sqrtf(velocity->x * velocity->x + velocity->y * velocity->y);
        if (vector_length != 0) {
            dx = velocity->x / vector_length;
            dy = velocity->y / vector_length;
        } else {
            dx = 0.0;
            dy = 0.0;
        }

        if (velocity->x > 0) {
            velocity->x -= dx * delta_time * PLAYER_BRAKING;
            if (velocity->x < 0) velocity->x = 0;
        } else if (velocity->x < 0) {
            velocity->x -= dx * delta_time * PLAYER_BRAKING;
            if (velocity->x > 0) velocity->x = 0;
        }

        if (velocity->y > 0) {
            velocity->y -= dy * delta_time * PLAYER_BRAKING;
            if (velocity->y < 0) velocity->y = 0;
        } else if (velocity->y < 0) {
            velocity->y -= dy * delta_time * PLAYER_BRAKING;
            if (velocity->y > 0) velocity->y = 0;
        }
    }

    if (position->x < 1) {
        position->x = 1;
        velocity->x = 0;
    }

    if (position->y < 1) {
        position->y = 1;
        velocity->y = 0;
    }

    if (position->x + render->frame.width > VIRTUAL_WIDTH - 1) {
        position->x = VIRTUAL_WIDTH - render->frame.width - 1;
        velocity->x = 0;
    }

    if (position->y + render->frame.height > VIRTUAL_HEIGHT - 1) {
        position->y = VIRTUAL_HEIGHT - render->frame.height - 1;
        velocity->y = 0;
    }
}
