#include "ext/raylib.h"
#include "smetanka_engine.h"
#include "witch_game.h"
#include "witch_game_update.h"
#include "witch_render.h"
#include "witch_resources.h"
#include "witch_save.h"
#include "witch_start.h"

void init(Game *game) {
    game->shouldClose = 0;

    resourcesInit(game);

    smePlayMusicStream(game->resources.music[MUSIC_MAIN]);

    ecsCreateSpace();

    initComponents(&game->components);

    loadGame(game);

    initStartScreen(game);
}

void updateAndRender(Game *game, RenderTexture *screen, float deltaTime) {
    InputState input = inputRead();

    gameUpdate(&input, game, deltaTime);

    render(game, screen);
}

void unload(Game *game) {
    smeUnloadTexture(game->resources.sprites[SPRITESHEET_PLAYER]);
    smeUnloadTexture(game->resources.sprites[SPRITESHEET_ENEMY]);
    smeUnloadTexture(game->resources.sprites[BACKGROUND_URBAN]);
    smeUnloadTexture(game->resources.sprites[BACKGROUND_CLOUDS]);

    smeUnloadSound(game->resources.sounds[SOUND_SHOOT]);
    smeUnloadSound(game->resources.sounds[SOUND_EXPLOSION]);

    smeStopMusicStream(game->resources.music[MUSIC_MAIN]);
    smeUnloadMusicStream(game->resources.music[MUSIC_MAIN]);

    ecsDestroySpace();
}
