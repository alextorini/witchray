#include "smetanka_engine.h"
#include "witch_components.h"
#include "witch_render.h"
#include "witch_systems.h"

void render(Game *game) {
    system_render_entities(game->components[CMP_POSITION], game->components[CMP_RENDER]);
    system_render_text(game->components[CMP_POSITION], game->components[CMP_TEXT_RENDER]);
    draw_text_format(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){3, 23},
        9, 1, DARKGREEN, "SCORE: %d", game->score
    );
    draw_text_format(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){3, 43},
        9, 1, DARKGREEN, "HIGHSCORE: %d", game->highscore
    );
    draw_text_format(game->resources.fonts[FONT_MAIN], CLITERAL(Position){3, 3}, 9, 1, DARKGREEN, "%d", GetFPS());
    draw_text_format(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){3, 63}, 9, 1, DARKGREEN, "%d", ecs_get_component_count(game->components[CMP_FIREBALL])
    );
}
