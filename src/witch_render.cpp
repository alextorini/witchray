#include "smetanka_engine.h"
#include "witch_components.h"
#include "witch_render.h"
#include "witch_game.h"
#include "witch_systems.h"

#define UI_HEALTH_COLOR {172, 50, 50, 196}

static void renderUI(Game *game) {
    Health *health = getHealth(game->player.handle);

    if (health && health->current > 0.0f) {
        DrawRectangle(9, 1, 9 + (int)(100 * (health->current / health->max)), 11 , UI_HEALTH_COLOR);
        // smeDrawTextF(
        //     game->resources.fonts[FONT_MAIN],
        //     CLITERAL(Position){3, 3},
        //     9, 1, DARKGREEN, "HEALTH: %d/%d", (int)health->current, (int)health->max
        // );
    }

    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Vector2){500, 3},
        9, 1, DARKGREEN, "SCORE: %d", game->score
    );
    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Vector2){500, 23},
        9, 1, DARKGREEN, "HIGHSCORE: %d", game->highscore
    );
}

static void renderDebug(Game *game) {
    smeDrawTextF(game->resources.fonts[FONT_MAIN], CLITERAL(Vector2){3, 350}, 9, 1, DARKGREEN, "FPS: %d", GetFPS());
    /* smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){3, 63}, 9, 1, DARKGREEN, "%d", ecsGetComponentCount(game->components[CMP_FIREBALL])
    ); */
}

void render(Game *game, RenderTexture *screen) {
    smeBeginTextureMode(*screen);

    clearBackground(game->skyColor);

    systemRenderEntities(&game->resources);
    systemRenderText();

    if (game->state == STATE_GAMEPLAY) {
        renderUI(game);
    }

    renderDebug(game);

    smeEndTextureMode();
}
