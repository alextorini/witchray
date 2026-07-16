#include <cmath>
#include "witch_input_read.h"

#define GAMEPAD_DEADZONE 0.1f

InputState inputRead() {
    InputState inputState;
    inputState.anyKey = (GetKeyPressed() != 0 ||
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) ||
        GetGamepadButtonPressed() != 0);

    inputState.fullscreenToggle = IsKeyPressed(KEY_F);

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
        float vectorLength = sqrtf(dx * dx + dy * dy);
        dx = dx / vectorLength;
        dy = dy / vectorLength;
    }

    inputState.move.x = dx;
    inputState.move.y = dy;

    return inputState;
}
