#include "smetanka_ecs.h"
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
        game->components[CMP_ENTITY_STATE],
        &game->resources
    );
    systemRenderText(game->components[CMP_POSITION], game->components[CMP_TEXT_RENDER]);

    Health *health = NULL;
    if (game->state != STATE_START_SCREEN) {
        health = (Health *)ecsGetEntityComponent(game->components[CMP_HEALTH], game->player.handle);
    }

    if (health) {
        smeDrawTextF(
            game->resources.fonts[FONT_MAIN],
            CLITERAL(Position){3, 3},
            9, 1, DARKGREEN, "HEALTH: %d/%d", (int)health->current, (int)health->max
        );
    }

    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){500, 3},
        9, 1, DARKGREEN, "SCORE: %d", game->score
    );
    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){500, 23},
        9, 1, DARKGREEN, "HIGHSCORE: %d", game->highscore
    );
    // smeDrawTextF(game->resources.fonts[FONT_MAIN], CLITERAL(Position){3, 3}, 9, 1, DARKGREEN, "%d", GetFPS());
    /* smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){3, 63}, 9, 1, DARKGREEN, "%d", ecsGetComponentCount(game->components[CMP_FIREBALL])
    ); */

    smeEndTextureMode();
}
