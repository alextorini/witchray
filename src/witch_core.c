#include "smetanka_engine.h"
#include "witch_game_update.h"
#include "witch_render.h"
#include "witch_resources.h"
#include "witch_save.h"
#include "witch_start.h"

uint8_t should_close;

void init(Game *game) {
    should_close = 0;

    resources_init(game);

    play_music_stream(game->resources.music[MUSIC_MAIN]);

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
    unload_texture(game->resources.sprites[SPRITESHEET_PLAYER]);
    unload_texture(game->resources.sprites[SPRITESHEET_ENEMY]);
    unload_texture(game->resources.sprites[BACKGROUND_URBAN]);
    unload_texture(game->resources.sprites[BACKGROUND_CLOUDS]);

    unload_sound(game->resources.sounds[SOUND_SHOOT]);
    unload_sound(game->resources.sounds[SOUND_EXPLOSION]);

    stop_music_stream(game->resources.music[MUSIC_MAIN]);
    unload_music_stream(game->resources.music[MUSIC_MAIN]);

    ecs_destroy_space();
}
