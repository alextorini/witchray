#include <math.h>
#include <stdint.h>

#include "ext/raylib.h"

#include "smetanka_ecs.h"
#include "witch_core.h"

#define PLAYER_MAX_VELOCITY 200.0
#define PLAYER_ACCELERATION 500.0
#define PLAYER_BRAKING  250.0

void process_input(float delta_time) {
    EcsEntityHandle plr_id = ecs_handles.entts.plr;
    Velocity *plr_vel = (Velocity *)ecs_get_entity_component(ecs_handles.cmpnts.vel, plr_id);
    Position *plr_pos = (Position *)ecs_get_entity_component(ecs_handles.cmpnts.pos, plr_id);
    Render *plr_rndr = (Render *)ecs_get_entity_component(ecs_handles.cmpnts.rndr, plr_id);

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

    if (dx != 0 || dy != 0) {
        plr_vel->x += dx * delta_time * PLAYER_ACCELERATION;
        plr_vel->y += dy * delta_time * PLAYER_ACCELERATION;

        float velocity = sqrtf(plr_vel->x * plr_vel->x + plr_vel->y * plr_vel->y);
        if (velocity > PLAYER_MAX_VELOCITY) {
            float ratio = PLAYER_MAX_VELOCITY / velocity;
            plr_vel->x *= ratio;
            plr_vel->y *= ratio;
        }
    } else {
        float vector_length = sqrtf(plr_vel->x * plr_vel->x + plr_vel->y * plr_vel->y);
        if (vector_length != 0) {
            dx = plr_vel->x / vector_length;
            dy = plr_vel->y / vector_length;
        } else {
            dx = 0.0;
            dy = 0.0;
        }

        if (plr_vel->x > 0) {
            plr_vel->x -= dx * delta_time * PLAYER_BRAKING;
            if (plr_vel->x < 0) plr_vel->x = 0;
        } else if (plr_vel->x < 0) {
            plr_vel->x -= dx * delta_time * PLAYER_BRAKING;
            if (plr_vel->x > 0) plr_vel->x = 0;
        }

        if (plr_vel->y > 0) {
            plr_vel->y -= dy * delta_time * PLAYER_BRAKING;
            if (plr_vel->y < 0) plr_vel->y = 0;
        } else if (plr_vel->y < 0) {
            plr_vel->y -= dy * delta_time * PLAYER_BRAKING;
            if (plr_vel->y > 0) plr_vel->y = 0;
        }
    }

    if (plr_pos->x < 1) {
        plr_pos->x = 1;
        plr_vel->x = 0;
    }

    if (plr_pos->y < 1) {
        plr_pos->y = 1;
        plr_vel->y = 0;
    }

    if (plr_pos->x + plr_rndr->frame.width > VIRTUAL_WIDTH - 1) {
        plr_pos->x = VIRTUAL_WIDTH - plr_rndr->frame.width - 1;
        plr_vel->x = 0;
    }

    if (plr_pos->y + plr_rndr->frame.height > VIRTUAL_HEIGHT - 1) {
        plr_pos->y = VIRTUAL_HEIGHT - plr_rndr->frame.height - 1;
        plr_vel->y = 0;
    }
}
