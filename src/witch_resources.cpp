#include "smetanka_engine.h"
#include "witch_game.h"
#include "witch_resources.h"

void resourcesInit(Game *game) {
    game->resources.fonts[FONT_MAIN] = smeloadFont("fonts/monocraft.otf");
    game->resources.sprites[SPRITESHEET_PLAYER] = smeLoadPixelTexture(PLAYER_IMAGE_PATH);
    game->resources.sprites[SPRITESHEET_ENEMY] = smeLoadPixelTexture(ENEMY_IMAGE_PATH);
    game->resources.sprites[SPRITESHEET_FIREBALL] = smeLoadPixelTexture(FIREBALL_IMAGE_PATH);
    game->resources.sprites[BACKGROUND_URBAN] = smeLoadPixelTexture("images/background_urban.png");
    game->resources.sprites[BACKGROUND_CLOUDS] = smeLoadPixelTexture("images/background_clouds.png");

    game->resources.sounds[SOUND_SHOOT] = smeLoadSound("sounds/shoot.wav");
    game->resources.sounds[SOUND_EXPLOSION] = smeLoadSound("sounds/explosion.wav");

    game->resources.music[MUSIC_MAIN] = smeLoadMusicStream("music/music2.wav");
    game->resources.music[MUSIC_MAIN].looping = true;
}
