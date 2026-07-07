#include "smetanka_engine.h"
#include "witch_game.h"
#include "witch_resources.h"

void resources_init(Game *game) {
    game->resources.fonts[FONT_MAIN] = load_font("fonts/monocraft.otf");
    game->resources.sprites[SPRITESHEET_PLAYER] = load_pixel_texture(PLAYER_IMAGE_PATH);
    game->resources.sprites[SPRITESHEET_ENEMY] = load_pixel_texture(ENEMY_IMAGE_PATH);
    game->resources.sprites[SPRITESHEET_FIREBALL] = load_pixel_texture(FIREBALL_IMAGE_PATH);
    game->resources.sprites[BACKGROUND_URBAN] = load_pixel_texture("images/background_urban.png");
    game->resources.sprites[BACKGROUND_CLOUDS] = load_pixel_texture("images/background_clouds.png");

    game->resources.sounds[SOUND_SHOOT] = load_sound("sounds/shoot.wav");
    game->resources.sounds[SOUND_EXPLOSION] = load_sound("sounds/explosion.wav");

    game->resources.music[MUSIC_MAIN] = load_music_stream("music/music2.wav");
    game->resources.music[MUSIC_MAIN].looping = true;
}
