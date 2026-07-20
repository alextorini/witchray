#include "witch_game.h"
#include "witch_input_read.h"
#include "witch_start.h"
#include "witch_state.h"

#define MAX_TEXT_LENGTH 128
#define START_SCREEN_TEXT_COLOR WHITE
#define START_SCREEN_TEXT_FONT game->resources.fonts[FONT_MAIN]
#define START_SCREEN_TEXT_FONT_SIZE 36
#define START_SCREEN_TEXT_FONT_SPACING 1
#define START_SCREEN_TEXT "Press Any Key To Start"

void startScreenInit(Game *game) {
    EcsEntityHandle startLabelHandle = ecsCreateEntity();

    Position startLabelPosition = {.x = 40, .y = 150};
    ecsAddComponent(startLabelHandle, game->components[CMP_POSITION], &startLabelPosition);

    TextRender start_label_text_render = {
        .offset = {0, 0},
        .color = START_SCREEN_TEXT_COLOR,
        .font = START_SCREEN_TEXT_FONT,
        .fontSize = START_SCREEN_TEXT_FONT_SIZE,
        .spacing = START_SCREEN_TEXT_FONT_SPACING,
        .text = START_SCREEN_TEXT
    };

    ecsAddComponent(startLabelHandle, game->components[CMP_TEXT_RENDER], &start_label_text_render);

    game->score = 0;
    game->state = STATE_START_SCREEN;
}

static void startScreenInputProcess(InputState *input, Game *game) {
    if (input->anyKey) {
        stateChange(game, STATE_GAMEPLAY);
    }
}

void startScreenUpdate(InputState *input, Game *game, float dt) {
    startScreenInputProcess(input, game);
}


void destroyStartScreen() {
    ecsClearSpace();
}
