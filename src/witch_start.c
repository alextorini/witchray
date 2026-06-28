#include "ext/raylib.h"
#include "smetanka_ecs.h"
#include "witch_components.h"
#include "witch_core.h"
#include "witch_resources.h"

#define MAX_TEXT_LENGTH 128
#define START_SCREEN_TEXT_COLOR WHITE
#define START_SCREEN_TEXT_FONT game.resources.fonts[FONT_MAIN]
#define START_SCREEN_TEXT_FONT_SIZE 36
#define START_SCREEN_TEXT_FONT_SPACING 1
#define START_SCREEN_TEXT "Press Any Key To Start"


void init_start_screen() {

    EcsEntityHandle start_label_handle = ecs_create_entity();

    Position start_label_position = {.x = 40, .y = 150};
    ecs_add_component(start_label_handle, game.components[CMP_POSITION], &start_label_position);

    TextRender start_label_text_render = {
        .offset = {0, 0},
        .color = START_SCREEN_TEXT_COLOR,
        .font = START_SCREEN_TEXT_FONT,
        .font_size = START_SCREEN_TEXT_FONT_SIZE,
        .spacing = START_SCREEN_TEXT_FONT_SPACING,
        .text = START_SCREEN_TEXT
    };

    ecs_add_component(start_label_handle, game.components[CMP_TEXT_RENDER], &start_label_text_render);
}

void destroy_start_screen() {
    ecs_clear_space();
}
