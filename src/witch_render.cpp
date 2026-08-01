#include "ext/raylib.h"
#include "smetanka_engine.h"
#include "witch_components.h"
#include "witch_render.h"
#include "witch_game.h"
#include "witch_systems.h"

#define UI_HEALTH_COLOR {172, 50, 50, 196}
#define UI_EXP_COLOR {50, 172, 50, 196}

static void renderUI(Game *game) {
    Health *health = getHealth(game->player.handle);

    if (health && health->current > 0.0f) {
        DrawRectangle(3, 1, 9 + (int)(100 * (health->current / health->max)), 11 , UI_HEALTH_COLOR);
        // smeDrawTextF(
        //     game->resources.fonts[FONT_MAIN],
        //     CLITERAL(Position){3, 3},
        //     9, 1, DARKGREEN, "HEALTH: %d/%d", (int)health->current, (int)health->max
        // );
    }

    if (game->player.exp > 0.0f) {
        float exp_percentage = game->player.exp / game->player.max_exp;
        DrawRectangle(0, 357, 640 * exp_percentage, 2 , UI_EXP_COLOR);
    }

    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Vector2){3, 345},
        9, 1, DARKGREEN, "Level %d", game->player.level
    );

    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Vector2){300, 3},
        9, 1, WHITE, "%d:%s%d", (int)(game->timer / 60), (int)game->timer%60 < 10 ? "0" : "", (int)game->timer % 60
    );

    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Vector2){510, 3},
        9, 1, DARKGREEN, "SCORE:     %d", game->score
    );
    smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Vector2){510, 18},
        9, 1, DARKGREEN, "HIGHSCORE: %d", game->highscore
    );
}

static void renderDebug(Game *game) {
    smeDrawTextF(game->resources.fonts[FONT_MAIN], CLITERAL(Vector2){580, 345}, 9, 1, DARKGREEN, "FPS: %d", GetFPS());
    /* smeDrawTextF(
        game->resources.fonts[FONT_MAIN],
        CLITERAL(Position){3, 63}, 9, 1, DARKGREEN, "%d", ecsGetComponentCount(game->components[CMP_FIREBALL])
    ); */
}

void render(Game *game, RenderTexture *screen) {
    smeBeginTextureMode(*screen);

    clearBackground(kColors[game->skyColor]);

    systemRenderEntities(&game->resources);
    systemRenderText();

    if (game->state == STATE_GAMEPLAY) {
        renderUI(game);
    }

    renderDebug(game);

    smeEndTextureMode();
}
