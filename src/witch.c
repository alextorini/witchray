#include "witch.h"
#include "raylib.h"
#include "raymath.h"
#include "render.h"
#include "secs.h"

static Font font;
static Music music;

#define SKY_COLOR CLITERAL(Color){255, 128, 156, 255}

#define BG_LAYER_1_SPEED 50.0f
#define BG_LAYER_2_SPEED 100.0f

static Background background;

typedef struct {
    Texture player;
    Texture enemy;
} Spritesheets;

static Spritesheets spritesheets;

typedef struct {
    Sound shoot;
    Sound explosion;
} Sounds;

static Sounds sounds;

static EcsSpace *space;

EcsPointers ecs_pointers;

void init() {
    int codepoints[512] = {0};
    for (int i = 0; i < 95; i++)
        codepoints[i] = 32 + i;
    for (int i = 0; i < 255; i++)
        codepoints[96 + i] = 0x400 + i;
    font = LoadFontEx("fonts/monocraft.otf", 18, codepoints, 512);

    spritesheets.player = load_pixel_texture("images/player.png");
    spritesheets.enemy = load_pixel_texture("images/enemy.png");
    background.layer_1 = load_pixel_texture("images/background1.png");
    background.layer_2 = load_pixel_texture("images/background2.png");

    sounds.shoot = LoadSound("sounds/shoot.wav");
    sounds.explosion = LoadSound("sounds/explosion.wav");

    music = LoadMusicStream("music/music.wav");
    music.looping = true;

    PlayMusicStream(music);

    space = ecs_create_space();

    ecs_pointers.components.position = ecs_register_component(space, (char *)"Position", sizeof(Position));
    ecs_pointers.components.velocity = ecs_register_component(space, (char *)"Velocity", sizeof(Velocity));
    ecs_pointers.components.render = ecs_register_component(space, (char *)"Render", sizeof(Render));
    ecs_pointers.components.paralax = ecs_register_component(space, (char *)"Paralax", sizeof(IsParalax));

    add_parallax_background_layer(space, &background.layer_1, BG_LAYER_1_SPEED);
    add_parallax_background_layer(space, &background.layer_2, BG_LAYER_2_SPEED);

    ecs_pointers.entities.player = ecs_create_entity(space);
    Position temp_position = {50, 50};
    ecs_add_component(ecs_pointers.entities.player, ecs_pointers.components.position, &temp_position);
    Render temp_render = {&spritesheets.player, {0.0f, 0.0f, 41.0f, 27.0f}};
    ecs_add_component(ecs_pointers.entities.player, ecs_pointers.components.render, &temp_render);
}

void update_and_draw() {
    UpdateMusicStream(music);

    ClearBackground(SKY_COLOR);

    Render *entity_render;
    Position *entity_position;
    Velocity *entity_velocity;
    EcsEntity *ep;

    Position tmp_pos;
    for (int i = 0; i < space->current_entity_id; i++) {
        ep = (EcsEntity *)space->entities + i;

        entity_position = (Position *)ecs_pointers.components.position->data + i;
        entity_velocity = (Velocity *)ecs_pointers.components.velocity->data + i;
        entity_render = (Render *)ecs_pointers.components.render->data + i;

        if (ep->comp_mask &
            (ecs_pointers.components.position->mask_bit | ecs_pointers.components.velocity->mask_bit)) {
            *entity_position = Vector2Add(*entity_position, Vector2Scale(*entity_velocity, GetFrameTime()));

            if ((ep->comp_mask & ecs_pointers.components.paralax->mask_bit) &&
                (entity_position->x + entity_render->frame.width < 0)) {
                entity_position->x += entity_render->frame.width * 2.0f;
            }
        }

        if (ep->comp_mask & (ecs_pointers.components.position->mask_bit | ecs_pointers.components.render->mask_bit)) {
            tmp_pos.x = (float)(int)floorf(entity_position->x);
            tmp_pos.y = (float)(int)floorf(entity_position->y);
            DrawTextureRec(*entity_render->spritesheet, entity_render->frame, tmp_pos, WHITE);
        }
    }

    DrawTextEx(font, TextFormat("%d", GetFPS()), CLITERAL(Position){3, 3}, 9, 1, DARKGREEN);
}

void unload() {
    UnloadTexture(spritesheets.player);
    UnloadTexture(spritesheets.enemy);
    UnloadTexture(background.layer_1);
    UnloadTexture(background.layer_2);

    UnloadSound(sounds.shoot);
    UnloadSound(sounds.explosion);

    StopMusicStream(music);
    UnloadMusicStream(music);

    ecs_destroy_space(space);
}
