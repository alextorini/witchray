#include <stdlib.h>
#include "witch_collisions.h"

#define COL_MALLOC malloc
#define COL_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define COL_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define COL_CALLOC calloc
#define COL_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define COL_FREE free

void init_collision_map(const char *image_path, Rectangle frame, uint32_t frames_count, FrameCollisionMap *col_map) {
    Image image = LoadImage(image_path);

    Color *pixels = LoadImageColors(image);
    uint32_t img_w = (uint32_t)image.width;
    uint32_t img_h = (uint32_t)image.height;

    uint8_t *image_mask = COL_MALLOC_ARR(uint8_t, img_w * img_h);
    for (uint32_t i = 0; i < img_w * img_h; i++) {
        image_mask[i] = pixels[i].a > 0;
    }

    col_map->width = (uint32_t)frame.width;
    col_map->height = (uint32_t)frame.height;
    col_map->count = frames_count;

    uint32_t frame_pixels_count = col_map->width * col_map->height;
    col_map->frame_mask_list = COL_MALLOC_ARR(uint8_t, frame_pixels_count * frames_count);
    for (uint32_t f = 0; f < frames_count; f++) {
        for (uint32_t y = 0; y < col_map->height; y++) {
            for (uint32_t x = 0; x < col_map->width; x++) {
                uint32_t src_x = (uint32_t)frame.x + (f * col_map->width) % img_w + x;
                uint32_t src_y = (uint32_t)frame.y + ((f * col_map->width) / img_w) * col_map->height + y;
                uint8_t mask = 0;
                if (src_x < img_w && src_y < img_h) {
                    mask = image_mask[src_y * img_w + src_x];
                }
                uint32_t dst_index = f * frame_pixels_count + y * col_map->width + x;
                col_map->frame_mask_list[dst_index] = mask;
            }
        }
    }

    COL_FREE(image_mask);
    UnloadImageColors(pixels);
    UnloadImage(image);
}

uint8_t pixel_perfect_collision(
    const FrameCollisionMap *a_col, uint32_t a_frame_index, float ax, float ay,
    const FrameCollisionMap *b_col, uint32_t b_frame_index, float bx, float by
) {
    if (!a_col || !b_col) return 0;
    if (a_frame_index >= a_col->count) return 0;
    if (b_frame_index >= b_col->count) return 0;

    int a_left = (int)ax;
    int a_top = (int)ay;
    int a_right = a_left + (int)a_col->width;
    int a_bottom = a_top + (int)a_col->height;

    int b_left = (int)bx;
    int b_top = (int)by;
    int b_right = b_left + (int)b_col->width;
    int b_bottom = b_top + (int)b_col->height;

    if (!(a_left < b_right && a_right > b_left && a_top < b_bottom && a_bottom > b_top)) return 0;

    int cx1 = a_left > b_left ? a_left : b_left;
    int cy1 = a_top > b_top ? a_top : b_top;
    int cx2 = a_right < b_right ? a_right : b_right;
    int cy2 = a_bottom < b_bottom ? a_bottom : b_bottom;

    uint32_t frame_pixels_a = a_col->width * a_col->height;
    uint32_t frame_pixels_b = b_col->width * b_col->height;
    uint32_t base_a = a_frame_index * frame_pixels_a;
    uint32_t base_b = b_frame_index * frame_pixels_b;

    for (int y = cy1; y < cy2; y++) {
        for (int x = cx1; x < cx2; x++) {
            uint32_t ax_local = (uint32_t)(x - a_left);
            uint32_t ay_local = (uint32_t)(y - a_top);
            uint32_t bx_local = (uint32_t)(x - b_left);
            uint32_t by_local = (uint32_t)(y - b_top);

            uint32_t idx_a = base_a + ay_local * a_col->width + ax_local;
            uint32_t idx_b = base_b + by_local * b_col->width + bx_local;

            if (a_col->frame_mask_list[idx_a] && b_col->frame_mask_list[idx_b]) {
                return 1;
            }
        }
    }

    return 0;
}
