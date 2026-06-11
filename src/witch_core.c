#include "ext/raylib.h"

#include "witch_render.h"
#include "witch_core.h"
#include "witch_systems.h"
#include "smetanka_misc.h"

static Font fnt;
static Music music;

#define SKY_COLOR CLITERAL(Color){255, 128, 156, 255}

#define BG_LAYER_1_SPEED 50.0f
#define BG_LAYER_2_SPEED 100.0f

static Background bckgrnd;

typedef struct {
    Texture player;
    Texture enemy;
} Spritesheets;

static Spritesheets sprtshts;

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
    fnt = LoadFontEx("fonts/monocraft.otf", 18, codepoints, 512);

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
}

void update_and_draw() {
    UpdateMusicStream(music);

    ClearBackground(SKY_COLOR);

    Render *e_rndr;
    Position *e_pos;
    Velocity *e_vel;
    IsParallax *e_is_prlx;
    EcsEntity *e_ptr;

    system_move_entities();
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
