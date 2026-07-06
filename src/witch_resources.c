#include "witch_game.h"
#include "witch_resources.h"

static Texture load_pixel_texture(const char *path) {
    Texture texture = LoadTexture(path);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    return texture;
}

static Font load_font(const char *path) {
    int codepoints[512] = {0};
    for (int i = 0; i < 95; i++)
        codepoints[i] = 32 + i;
    for (int i = 0; i < 255; i++)
        codepoints[96 + i] = 0x400 + i;

    return LoadFontEx(path, 18, codepoints, 350);
}

void resources_init(Game *game) {
    game->resources.fonts[FONT_MAIN] = load_font("fonts/monocraft.otf");
    game->resources.sprites[SPRITESHEET_PLAYER] = load_pixel_texture(PLAYER_IMAGE_PATH);
    game->resources.sprites[SPRITESHEET_ENEMY] = load_pixel_texture(ENEMY_IMAGE_PATH);
    game->resources.sprites[SPRITESHEET_FIREBALL] = load_pixel_texture(FIREBALL_IMAGE_PATH);
    game->resources.sprites[BACKGROUND_URBAN] = load_pixel_texture("images/background_urban.png");
    game->resources.sprites[BACKGROUND_CLOUDS] = load_pixel_texture("images/background_clouds.png");

    game->resources.sounds[SOUND_SHOOT] = LoadSound("sounds/shoot.wav");
    game->resources.sounds[SOUND_EXPLOSION] = LoadSound("sounds/explosion.wav");

    game->resources.music[MUSIC_MAIN] = LoadMusicStream("music/music.wav");
    game->resources.music[MUSIC_MAIN].looping = true;
}
