#include "ext/raylib.h"

#include "witch_core.h"
#include "smetanka_ecs.h"
#include "witch_parallax.h"
#include "witch_systems.h"
#include "smetanka_misc.h"
#include "smetanka_render.h"
#include "witch_enemies.h"
#include "witch_player.h"


static Font fnt;
static Music music;

static Background bckgrnd;

static AnimationClip plr_idle;
static AnimationSet plr_anim_set;

typedef struct {
    Sound shoot;
    Sound explosion;
} Sounds;

static Sounds snds;

Spritesheets sprtshts;

EcsHandles ecs_hndls;

void init() {
    fnt = load_font("fonts/monocraft.otf");

    sprtshts.player = load_pixel_texture("images/player.png");
    sprtshts.enemy = load_pixel_texture("images/enemy.png");
    bckgrnd.layer_1 = load_pixel_texture("images/background1.png");
    bckgrnd.layer_2 = load_pixel_texture("images/background2.png");

    snds.shoot = LoadSound("sounds/shoot.wav");
    snds.explosion = LoadSound("sounds/explosion.wav");

    music = LoadMusicStream("music/music.wav");
    music.looping = true;

    PlayMusicStream(music);

    ecs_create_space();

    ecs_hndls.cmpnts.pos = ecs_register_component((char *)"Position", sizeof(Position));
    ecs_hndls.cmpnts.vel = ecs_register_component((char *)"Velocity", sizeof(Velocity));
    ecs_hndls.cmpnts.rndr = ecs_register_component((char *)"Render", sizeof(Render));
    ecs_hndls.cmpnts.anim = ecs_register_component((char *)"Animation", sizeof(Animation));
    ecs_hndls.cmpnts.prlx = ecs_register_component((char *)"Parallax", sizeof(IsParallax));
    ecs_hndls.cmpnts.enmy = ecs_register_component((char *)"Enemy", sizeof(IsEnemy));

    EcsEntityHandle *layer_copies;
    layer_copies = add_parallax_background_layer(&bckgrnd.layer_1, BG_LAYER_1_SPEED);
    ecs_hndls.entts.background_layers[0][0] = layer_copies[0];
    ecs_hndls.entts.background_layers[0][1] = layer_copies[1];
    layer_copies = add_parallax_background_layer(&bckgrnd.layer_2, BG_LAYER_2_SPEED);
    ecs_hndls.entts.background_layers[1][0] = layer_copies[0];
    ecs_hndls.entts.background_layers[1][1] = layer_copies[1];

    ecs_hndls.entts.plr = ecs_create_entity();
    Position plr_pos = PLAYER_START_POS;
    ecs_add_component(ecs_hndls.entts.plr, ecs_hndls.cmpnts.pos, &plr_pos);
    Render plr_rndr = {&sprtshts.player, PLAYER_DEFAULT_FRAME};
    ecs_add_component(ecs_hndls.entts.plr, ecs_hndls.cmpnts.rndr, &plr_rndr);
    Velocity plr_vel = {0.0, 0.0};
    ecs_add_component(ecs_hndls.entts.plr, ecs_hndls.cmpnts.vel, &plr_vel);

    plr_idle.start_frame = 0;
    plr_idle.end_frame = 1;
    plr_idle.frame_time = PLAYER_IDLE_ANIM_SPEED;
    plr_idle.loop = 1;

    plr_anim_set.clips = WR_MALLOC_TYPE(AnimationClip);
    plr_anim_set.count = 1;
    plr_anim_set.clips[0] = plr_idle;

    Animation plr_anim;
    plr_anim.set = &plr_anim_set;
    plr_anim.current_clip = 0;
    plr_anim.current_frame = 0;
    plr_anim.timer = 0.0;
    ecs_add_component(ecs_hndls.entts.plr, ecs_hndls.cmpnts.anim, &plr_anim);

    init_enemy_factory();
}

void update_and_draw() {
    UpdateMusicStream(music);

    ClearBackground(SKY_COLOR);

    float delta_time = GetFrameTime();

    process_input(delta_time);
    system_spawn_enemies(delta_time);
    system_clean_enemies();
    system_move_entities(delta_time);
    system_animate_entities(delta_time);
    system_render_entities();
    system_move_parallax();

    DrawTextEx(fnt, TextFormat("%d", GetFPS()), CLITERAL(Position){3, 3}, 9, 1, DARKGREEN);
    DrawTextEx(fnt, TextFormat("%d", ecs_get_component_count(ecs_hndls.cmpnts.enmy)), CLITERAL(Position){3, 33}, 9, 1, DARKGREEN);
}


void unload() {
    UnloadTexture(sprtshts.player);
    UnloadTexture(sprtshts.enemy);
    UnloadTexture(bckgrnd.layer_1);
    UnloadTexture(bckgrnd.layer_2);

    UnloadSound(snds.shoot);
    UnloadSound(snds.explosion);

    StopMusicStream(music);
    UnloadMusicStream(music);

    ecs_destroy_space();
}
