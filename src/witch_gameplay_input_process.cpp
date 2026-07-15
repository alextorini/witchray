#include "witch_gameplay_input_process.h"
#include "witch_input_read.h"

#define PLAYER_MAX_VELOCITY 200.0
#define PLAYER_ACCELERATION 500.0
#define PLAYER_BRAKING  250.0

void gameplayInputProcess(InputState *input, Game *game, float dt) {
    // TODO: Get rid of all this components dependencies.
    // Maybe this function should only generate something like forces,
    // which will applied to the player in some other system
    Velocity *velocity = (Velocity *)ecsGetEntityComponent(game->components[CMP_VELOCITY], game->player.handle);
    Position *position = (Position *)ecsGetEntityComponent(game->components[CMP_POSITION], game->player.handle);
    Render *render = (Render *)ecsGetEntityComponent(game->components[CMP_RENDER], game->player.handle);

    float moveMagnitude;
    if ( input->move.x != 0 || input->move.y != 0) {
        velocity->x += input->move.x * dt * PLAYER_ACCELERATION;
        velocity->y += input->move.y * dt * PLAYER_ACCELERATION;

        moveMagnitude = xyMagnitude(velocity->x, velocity->y);
        if (moveMagnitude > PLAYER_MAX_VELOCITY) {
            float ratio = PLAYER_MAX_VELOCITY / moveMagnitude;
            velocity->x *= ratio;
            velocity->y *= ratio;
        }
    } else {
        float dx, dy;
        moveMagnitude = xyMagnitude(velocity->x, velocity->y);
        if (moveMagnitude != 0) {
            dx = velocity->x / moveMagnitude;
            dy = velocity->y / moveMagnitude;
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
