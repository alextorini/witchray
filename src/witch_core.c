#include <stdlib.h>

#include "ext/raylib.h"

#include "witch_render.h"
#include "witch_core.h"
#include "witch_systems.h"
#include "smetanka_misc.h"

#define WR_MALLOC malloc
#define WR_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define WR_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define WR_CALLOC calloc
#define WR_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define WR_FREE free

static Font fnt;
static Music music;

#define SKY_COLOR CLITERAL(Color){255, 128, 156, 255}

#define BG_LAYER_1_SPEED 50.0f
#define BG_LAYER_2_SPEED 150.0f

static Background bckgrnd;

typedef struct {
    Texture player;
    Texture enemy;
} Spritesheets;

static Spritesheets sprtshts;

static AnimationClip plr_idle;
static AnimationSet plr_anim_set;

typedef struct {
    Sound shoot;
    Sound explosion;
} Sounds;

static Sounds snds;

EcsSpace *spc;

EcsPointers ecs_ptrs;

void init() {
    int codepoints[512] = {0};
    for (int i = 0; i < 95; i++)
        codepoints[i] = 32 + i;
    for (int i = 0; i < 255; i++)
        codepoints[96 + i] = 0x400 + i;
    fnt = LoadFontEx("fonts/monocraft.otf", 18, codepoints, 350);

    sprtshts.player = load_pixel_texture("images/player.png");
    sprtshts.enemy = load_pixel_texture("images/enemy.png");
    bckgrnd.layer_1 = load_pixel_texture("images/background1.png");
    bckgrnd.layer_2 = load_pixel_texture("images/background2.png");

    snds.shoot = LoadSound("sounds/shoot.wav");
    snds.explosion = LoadSound("sounds/explosion.wav");

    music = LoadMusicStream("music/music.wav");
    music.looping = true;

    PlayMusicStream(music);

    spc = ecs_create_space();

    ecs_ptrs.cmpnts.pos = ecs_register_component(spc, (char *)"Position", sizeof(Position));
    ecs_ptrs.cmpnts.vel = ecs_register_component(spc, (char *)"Velocity", sizeof(Velocity));
    ecs_ptrs.cmpnts.rndr = ecs_register_component(spc, (char *)"Render", sizeof(Render));
    ecs_ptrs.cmpnts.anim = ecs_register_component(spc, (char *)"Animation", sizeof(Animation));
    ecs_ptrs.cmpnts.prlx = ecs_register_component(spc, (char *)"Parallax", sizeof(IsParallax));

    EcsEntity **layer_copies;
    layer_copies = add_parallax_background_layer(spc, &bckgrnd.layer_1, BG_LAYER_1_SPEED);
    ecs_ptrs.entts.background_layers[0][0] = layer_copies[0];
    ecs_ptrs.entts.background_layers[0][1] = layer_copies[1];
    layer_copies = add_parallax_background_layer(spc, &bckgrnd.layer_2, BG_LAYER_2_SPEED);
    ecs_ptrs.entts.background_layers[1][0] = layer_copies[0];
    ecs_ptrs.entts.background_layers[1][1] = layer_copies[1];

    ecs_ptrs.entts.plr = ecs_create_entity(spc);
    Position temp_position = {50, 50};
    ecs_add_component(ecs_ptrs.entts.plr, ecs_ptrs.cmpnts.pos, &temp_position);
    Render temp_render = {&sprtshts.player, {0.0f, 0.0f, 41.0f, 27.0f}};
    ecs_add_component(ecs_ptrs.entts.plr, ecs_ptrs.cmpnts.rndr, &temp_render);

    plr_idle.start_frame = 0;
    plr_idle.end_frame = 1;
    plr_idle.frame_time = 0.25;
    plr_idle.loop = true;

    plr_anim_set.clips = WR_MALLOC_TYPE(AnimationClip);
    plr_anim_set.count = 1;
    plr_anim_set.clips[0] = plr_idle;

    Animation plr_anim;
    plr_anim.set = &plr_anim_set;
    plr_anim.current_clip = 0;
    plr_anim.current_frame = 0;
    plr_anim.timer = 0.0;
    ecs_add_component(ecs_ptrs.entts.plr, ecs_ptrs.cmpnts.anim, &plr_anim);
}

void update_and_draw() {
    UpdateMusicStream(music);

    ClearBackground(SKY_COLOR);

    system_move_entities();
    system_animate_entities();
    system_render_entities();
    system_move_parallax();

    DrawTextEx(fnt, TextFormat("%d", GetFPS()), CLITERAL(Position){3, 3}, 9, 1, DARKGREEN);
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

    ecs_destroy_space(spc);
}
