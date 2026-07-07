#pragma once

#include <stdint.h>
#include "ext/raylib.h"
#include "smetanka_math.h"

typedef Rectangle Rectangle;
typedef Sound Sound;
typedef Music Music;
typedef Font Font;
typedef Image Image;
typedef  Texture Texture;
typedef Color Color;

typedef struct {
    const char *window_name;
    int window_width;
    int window_height;
    int virtual_width;
    int virutal_height;
    unsigned int config_flags;
    int fps;
    const char *resource_dir;
    uint8_t fullscreen;
} SmetankaInitData;

void smetanka_init(SmetankaInitData *data);
bool window_should_close();
void smetanka_close();

void begin_texture_mode();
void end_texture_mode();

void smetanka_render();

float get_dt();

void draw_texture_rec(Texture texture, Rectangle source, Vector2 position, Color tint);
void draw_text(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);
void draw_text_format(
    Font font,
    Vector2 position,
    float font_size,
    float spacing,
    Color tint,
    const char *format,
    ...
);

Font load_font(const char *path);

Image load_image(const char *filename);
void unload_image(Image image);

Color *load_image_colors(Image image);
void unload_image_colors(Color *colors);

Texture load_texture(const char *filename);
Texture load_pixel_texture(const char *filename);
void unload_texture(Texture2D texture);

Sound load_sound(const char *filename);
void unload_sound(Sound sound);

Music load_music_stream(const char *filename);
void unload_music_stream(Music music);

void play_music_stream(Music music);
void stop_music_stream(Music music);

unsigned char *load_file_data(const char *filename, int *data_size);
void unload_file_data(unsigned char *data);
bool save_file_data(const char *filename, void *data, int data_size);
