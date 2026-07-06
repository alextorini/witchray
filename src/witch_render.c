#include "witch_components.h"
#include "witch_render.h"
#include "witch_systems.h"

void render(Game *game) {
    system_render_entities(game->components[CMP_POSITION], game->components[CMP_RENDER]);
    system_render_text(game->components[CMP_POSITION], game->components[CMP_TEXT_RENDER]);
    DrawTextEx(
        game->resources.fonts[FONT_MAIN],
        TextFormat("SCORE: %d", game->score),
        CLITERAL(Position){3, 23}, 9, 1, DARKGREEN
    );
    DrawTextEx(
        game->resources.fonts[FONT_MAIN],
        TextFormat("HIGHSCORE: %d", game->highscore),
        CLITERAL(Position){3, 43}, 9, 1, DARKGREEN
    );
    DrawTextEx(game->resources.fonts[FONT_MAIN], TextFormat("%d", GetFPS()), CLITERAL(Position){3, 3}, 9, 1, DARKGREEN);
    DrawTextEx(
        game->resources.fonts[FONT_MAIN],
        TextFormat("%d", ecs_get_component_count(game->components[CMP_FIREBALL])),
        CLITERAL(Position){3, 63}, 9, 1, DARKGREEN
    );
}
