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

void init_resources(ResourceMap *resource_map) {
    resource_map->fonts[FONT_MAIN] = load_font("fonts/monocraft.otf");
    resource_map->sprites[SPRITESHEET_PLAYER] = load_pixel_texture(PLAYER_IMAGE_PATH);
    resource_map->sprites[SPRITESHEET_ENEMY] = load_pixel_texture(ENEMY_IMAGE_PATH);
    resource_map->sprites[BACKGROUND_CLOUDS] = load_pixel_texture("images/background_clouds.png");
    resource_map->sprites[BACKGROUND_URBAN] = load_pixel_texture("images/background_urban.png");

    resource_map->sounds[SOUND_SHOOT] = LoadSound("sounds/shoot.wav");
    resource_map->sounds[SOUND_EXPLOSION] = LoadSound("sounds/explosion.wav");

    resource_map->music[MUSIC_MAIN] = LoadMusicStream("music/music.wav");
    resource_map->music[MUSIC_MAIN].looping = true;
}
