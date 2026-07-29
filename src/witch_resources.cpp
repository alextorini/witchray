#include "witch_resources.h"

void resourcesInit(ResourceRegistry *resourceRegistry) {
    resourceRegistry->fonts[FONT_MAIN] = smeloadFont("fonts/monocraft.otf");
    resourceRegistry->sprites[SPRITESHEET_PLAYER] = smeLoadPixelTexture(PLAYER_IMAGE_PATH);
    resourceRegistry->sprites[SPRITESHEET_ENEMY] = smeLoadPixelTexture(ENEMY_IMAGE_PATH);
    resourceRegistry->sprites[SPRITESHEET_FIREBALL] = smeLoadPixelTexture(FIREBALL_IMAGE_PATH);
    resourceRegistry->sprites[SPRITESHEET_ICEBALL] = smeLoadPixelTexture(ICEBALL_IMAGE_PATH);
    resourceRegistry->sprites[SPRITESHEET_STARBALL] = smeLoadPixelTexture(STARBALL_IMAGE_PATH);
    resourceRegistry->sprites[SPRITESHEET_EXP_CRYSTAL] = smeLoadPixelTexture(EXP_CRYSTAL_PATH );
    resourceRegistry->sprites[SPRITESHEET_HEALTH_CRYSTAL] = smeLoadPixelTexture("images/health_crystal.png");
    resourceRegistry->sprites[BACKGROUND_URBAN] = smeLoadPixelTexture("images/background_urban.png");
    resourceRegistry->sprites[BACKGROUND_CLOUDS] = smeLoadPixelTexture("images/background_clouds.png");

    resourceRegistry->sounds[SOUND_SHOOT] = smeLoadSound("sounds/shoot.wav");
    SetSoundVolume(resourceRegistry->sounds[SOUND_SHOOT], 0.3f);
    resourceRegistry->sounds[SOUND_EXPLOSION] = smeLoadSound("sounds/explosion.wav");
    SetSoundVolume(resourceRegistry->sounds[SOUND_EXPLOSION], 0.3f);
    resourceRegistry->sounds[SOUND_PLAYER_DEATH] = smeLoadSound("sounds/player_death.wav");
    SetSoundVolume(resourceRegistry->sounds[SOUND_PLAYER_DEATH], 0.5f);

    resourceRegistry->sounds[SOUND_PICKUP_EXP] = smeLoadSound("sounds/pickup_exp.wav");
    SetSoundVolume(resourceRegistry->sounds[SOUND_PICKUP_EXP], 0.2f);
    resourceRegistry->sounds[SOUND_PICKUP_HP] = smeLoadSound("sounds/pickup_health.wav");
    SetSoundVolume(resourceRegistry->sounds[SOUND_PICKUP_HP], 0.5f);

    resourceRegistry->sounds[SOUND_PAUSE] = smeLoadSound("sounds/pause.wav");
    SetSoundVolume(resourceRegistry->sounds[SOUND_PAUSE], 0.5f);

    resourceRegistry->music[MUSIC_MAIN] = smeLoadMusicStream("music/music2.wav");
    resourceRegistry->music[MUSIC_MAIN].looping = true;

    #if defined(PLATFORM_WEB)
    resourceMap->shaders.damageFlash.shader = smeLoadShader("shaders/web/damage_flash.glsl");
    #else
    resourceRegistry->shaders.damageFlash.shader = smeLoadShader("shaders/damage_flash.glsl");
    #endif

    resourceRegistry->shaders.damageFlash.colorLoc  =
        GetShaderLocation(resourceRegistry->shaders.damageFlash.shader, "flashColor");
    resourceRegistry->shaders.damageFlash.strengthLoc =
        GetShaderLocation(resourceRegistry->shaders.damageFlash.shader, "flashStrength");
}
