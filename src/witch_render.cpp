#include "smetanka_engine.h"
#include "witch_components.h"
#include "witch_render.h"
#include "witch_systems.h"

void render(Game *game, RenderTexture *screen) {
    smeBeginTextureMode(*screen);

    ClearBackground(SKY_COLOR);

    systemRenderEntities(
        game->components[CMP_POSITION],
        game->components[CMP_RENDER],
        game->components[CMP_ENTITY_STATE]
    );
    systemRenderText(game->components[CMP_POSITION], game->components[CMP_TEXT_RENDER]);

    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){3, 23},
        9, 1, DARKGREEN, "SCORE: %d", game->score
    );
    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){3, 43},
        9, 1, DARKGREEN, "HIGHSCORE: %d", game->highscore
    );
    smeDrawTextF(game->resources.fonts[FONT_MAIN], CLITERAL(Position){3, 3}, 9, 1, DARKGREEN, "%d", GetFPS());
    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){3, 63}, 9, 1, DARKGREEN, "%d", ecsGetComponentCount(game->components[CMP_FIREBALL])
    );

    smeEndTextureMode();
}
