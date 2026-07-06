#include "witch_game_update.h"
#include "witch_render.h"
#include "witch_resources.h"
#include "witch_save.h"
#include "witch_start.h"

uint8_t should_close;

void init(Game *game) {
    should_close = 0;

    resources_init(game);

    PlayMusicStream(game->resources.music[MUSIC_MAIN]);

    ecs_create_space();

    init_components(&game->components);

    load_game(game);

    init_start_screen(game);
}

void update_and_draw(Game *game, float delta_time) {
    InputState input = input_read();

    game_update(&input, game, delta_time);

    render(game);
}

void unload(Game *game) {
    UnloadTexture(game->resources.sprites[SPRITESHEET_PLAYER]);
    UnloadTexture(game->resources.sprites[SPRITESHEET_ENEMY]);
    UnloadTexture(game->resources.sprites[BACKGROUND_URBAN]);
    UnloadTexture(game->resources.sprites[BACKGROUND_CLOUDS]);

    UnloadSound(game->resources.sounds[SOUND_SHOOT]);
    UnloadSound(game->resources.sounds[SOUND_EXPLOSION]);

    StopMusicStream(game->resources.music[MUSIC_MAIN]);
    UnloadMusicStream(game->resources.music[MUSIC_MAIN]);

    ecs_destroy_space();
}
