#include "witch_gameplay_input_process.h"
#include "witch_input_read.h"

#define PLAYER_MAX_VELOCITY 200.0
#define PLAYER_ACCELERATION 500.0
#define PLAYER_BRAKING  250.0

void gameplay_input_process(InputState *input, Game *game, float dt) {
    // TODO: Get rid of all this components dependencies.
    // Maybe this function should only generate something like forces,
    // which will applied to the player in some other system
    Velocity *velocity = (Velocity *)ecs_get_entity_component(game->components[CMP_VELOCITY], game->player.handle);
    Position *position = (Position *)ecs_get_entity_component(game->components[CMP_POSITION], game->player.handle);
    Render *render = (Render *)ecs_get_entity_component(game->components[CMP_RENDER], game->player.handle);

    float move_magnitude;
    if ( input->move.x != 0 || input->move.y != 0) {
        velocity->x += input->move.x * dt * PLAYER_ACCELERATION;
        velocity->y += input->move.y * dt * PLAYER_ACCELERATION;

        move_magnitude = xy_magnitude(velocity->x, velocity->y);
        if (move_magnitude > PLAYER_MAX_VELOCITY) {
            float ratio = PLAYER_MAX_VELOCITY / move_magnitude;
            velocity->x *= ratio;
            velocity->y *= ratio;
        }
    } else {
        float dx, dy;
        move_magnitude = xy_magnitude(velocity->x, velocity->y);
        if (move_magnitude != 0) {
            dx = velocity->x / move_magnitude;
            dy = velocity->y / move_magnitude;
        } else {
            dx = 0.0;
            dy = 0.0;
        }

        if (velocity->x > 0) {
            velocity->x -= dx * dt * PLAYER_BRAKING;
            if (velocity->x < 0) velocity->x = 0;
        } else if (velocity->x < 0) {
            velocity->x -= dx * dt * PLAYER_BRAKING;
            if (velocity->x > 0) velocity->x = 0;
        }

        if (velocity->y > 0) {
            velocity->y -= dy * dt * PLAYER_BRAKING;
            if (velocity->y < 0) velocity->y = 0;
        } else if (velocity->y < 0) {
            velocity->y -= dy * dt * PLAYER_BRAKING;
            if (velocity->y > 0) velocity->y = 0;
        }
    }

    //TODO: Move this to a suitable module/system.
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
