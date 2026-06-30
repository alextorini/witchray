#include <stdlib.h>

#include "witch_collision.h"

#define COL_MALLOC malloc
#define COL_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define COL_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define COL_CALLOC calloc
#define COL_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define COL_FREE free

void init_collision_map(const char *image_path, Rectangle frame, uint32_t frame_count, FrameCollisionMap collision_map) {
    Image player_image = LoadImage(image_path);
    Color *pixels = COL_MALLOC_ARR(Color, player_image.width * player_image.height);
    pixels = LoadImageColors(player_image);
    uint8_t *image_mask = COL_MALLOC_ARR(uint8_t, player_image.width * player_image.height);
    for (int i = 0; i < player_image.width * player_image.height; i++) {
        image_mask[i] = pixels[i].a > 0;
    }
    COL_FREE(pixels);

    collision_map.mask_list = (uint8_t *)COL_MALLOC(sizeof(uint8_t) * frame.width * frame.height * frame_count);
    for (int i = 0; i < frame.width * frame.height; i++) {
        int frame_x = i % (int)frame.width;
        int frame_y = i / frame.width;
        int mask_x = frame_x;
        int mask_y = frame_y;
        int j = mask_y * player_image.width + mask_x;
        collision_map.mask_list[i] = image_mask[j];
    }
}
