#include "ext/raylib.h"

#include "smetanka_ecs.h"
#include "smetanka_misc.h"
#include "witch_core.h"
#include "witch_systems.h"
#include "witch_components.h"
#include "witch_enemies.h"
#include "witch_player.h"
#include "witch_resources.h"
#include "witch_start.h"

static AnimationClip plr_idle;
static AnimationSet plr_anim_set;

Game game;

uint8_t should_close;

void init() {
    should_close = 0;

    init_resources(&game.resources);

    PlayMusicStream(game.resources.music[MUSIC_MAIN]);

    ecs_create_space();
    init_components(&game.components);

    init_start_screen();
}

void update_and_draw() {
    UpdateMusicStream(game.resources.music[MUSIC_MAIN]);

    ClearBackground(SKY_COLOR);

    float delta_time = GetFrameTime();

    process_input(game.player_handle, delta_time);

    if (game.state == STATE_GAMEPLAY) {
        system_spawn_enemies(delta_time);
        system_clean_enemies(game.components[CMP_ENEMY], game.components[CMP_POSITION]);
    }
    system_move_entities(game.components[CMP_POSITION], game.components[CMP_VELOCITY], delta_time);
    system_animate_entities(game.components[CMP_ANIMATION], game.components[CMP_RENDER],delta_time);
    if (game.state == STATE_GAMEPLAY) {
        system_collide_enemies(
            game.player_handle,
            game.components[CMP_ENEMY],
            game.components[CMP_POSITION],
            game.components[CMP_RENDER]
        );
    }
    system_render_entities(game.components[CMP_POSITION], game.components[CMP_RENDER]);
    system_render_text(game.components[CMP_POSITION], game.components[CMP_TEXT_RENDER]);
    system_move_parallax(game.components[CMP_PARALLAX], game.components[CMP_POSITION], game.components[CMP_RENDER]);
    DrawTextEx(game.resources.fonts[FONT_MAIN], TextFormat("%d", GetFPS()), CLITERAL(Position){3, 3}, 9, 1, DARKGREEN);
    DrawTextEx(
        game.resources.fonts[FONT_MAIN],
        TextFormat("%d", ecs_get_component_count(game.components[CMP_ENEMY])),
        CLITERAL(Position){3, 33}, 9, 1, DARKGREEN
    );
}


void unload() {
    UnloadTexture(game.resources.sprites[SPRITESHEET_PLAYER]);
    UnloadTexture(game.resources.sprites[SPRITESHEET_ENEMY]);
    UnloadTexture(game.resources.sprites[BACKGROUND_URBAN]);
    UnloadTexture(game.resources.sprites[BACKGROUND_CLOUDS]);

    UnloadSound(game.resources.sounds[SOUND_SHOOT]);
    UnloadSound(game.resources.sounds[SOUND_EXPLOSION]);

    StopMusicStream(game.resources.music[MUSIC_MAIN]);
    UnloadMusicStream(game.resources.music[MUSIC_MAIN]);

    ecs_destroy_space();
}
