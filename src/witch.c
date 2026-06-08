#include "witch.h"
#include "raylib.h"
#include "raymath.h"
#include "render.h"
#include "secs.h"
#include "misc.h"


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

static EcsSpace *spc;

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
    ecs_ptrs.cmpnts.is_prlx = ecs_register_component(spc, (char *)"Paralax", sizeof(IsParalax));
    // ecs_ptrs.cmpnts.is_plr = ecs_register_component(spc, (char *)"Player", sizeof(IsPlayer));

    add_parallax_background_layer(spc, &bckgrnd.layer_1, BG_LAYER_1_SPEED);
    add_parallax_background_layer(spc, &bckgrnd.layer_2, BG_LAYER_2_SPEED);

    ecs_ptrs.entts.plr = ecs_create_entity(spc);
    Position temp_position = {50, 50};
    ecs_add_component(ecs_ptrs.entts.plr, ecs_ptrs.cmpnts.pos, &temp_position);
    Render temp_render = {&sprtshts.player, {0.0f, 0.0f, 41.0f, 27.0f}};
    ecs_add_component(ecs_ptrs.entts.plr, ecs_ptrs.cmpnts.rndr, &temp_render);
    /* IsPlayer temp_player = true;
    ecs_add_component(ecs_ptrs.entts.plr, ecs_ptrs.cmpnts.is_plr, &temp_player); */
}

void update_and_draw() {
    UpdateMusicStream(music);

    ClearBackground(SKY_COLOR);

    Render *e_rndr;
    Position *e_pos;
    Velocity *e_vel;
    IsParalax *e_is_prlx;
    // IsPlayer *e_is_plr;
    EcsEntity *e_ptr;
    for (int i = 0; i < spc->current_entity_id; i++) {
        e_ptr = (EcsEntity *)spc->entities + i;
        e_pos = (Position *)ecs_ptrs.cmpnts.pos->data + i;
        e_vel = (Velocity *)ecs_ptrs.cmpnts.vel->data + i;
        e_rndr = (Render *)ecs_ptrs.cmpnts.rndr->data + i;
        e_is_prlx = (IsParalax *)ecs_ptrs.cmpnts.is_prlx->data + i;
        // e_is_plr = (IsPlayer *)ecs_ptrs.cmpnts.is_plr->data + i;

        if (e_ptr->comp_mask & (ecs_ptrs.cmpnts.pos->mask_bit | ecs_ptrs.cmpnts.vel->mask_bit)) {
            *e_pos = Vector2Add(*e_pos, Vector2Scale(*e_vel, GetFrameTime()));

            if ((e_ptr->comp_mask & ecs_ptrs.cmpnts.is_prlx->mask_bit) && *e_is_prlx == 1 &&
                (e_pos->x + e_rndr->frame.width < 0)) {
                e_pos->x += e_rndr->frame.width * 2.0f;
            }
        }

        /* if ((e_ptr->comp_mask & ecs_ptrs.cmpnts.is_plr->mask_bit) && *e_is_plr == 1) {
            if (IsKeyDown(KEY_RIGHT)) e_pos->x += 2.0f;
            if (IsKeyDown(KEY_LEFT)) e_pos->x -= 2.0f;
            if (IsKeyDown(KEY_UP)) e_pos->y -= 2.0f;
            if (IsKeyDown(KEY_DOWN)) e_pos->y += 2.0f;
        } */
    }

    Position draw_pos;
    for (int i = 0; i < spc->current_entity_id; i++) {
        e_ptr = (EcsEntity *)spc->entities + i;
        e_pos = (Position *)ecs_ptrs.cmpnts.pos->data + i;
        e_rndr = (Render *)ecs_ptrs.cmpnts.rndr->data + i;
        if (e_ptr->comp_mask & (ecs_ptrs.cmpnts.pos->mask_bit | ecs_ptrs.cmpnts.rndr->mask_bit)) {
            draw_pos.x = (float)(int)floorf(e_pos->x);
            draw_pos.y = (float)(int)floorf(e_pos->y);
            DrawTextureRec(*e_rndr->spritesheet, e_rndr->frame, draw_pos, WHITE);
        }
    }

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
