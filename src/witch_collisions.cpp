#include <stdlib.h>
#include "smetanka_engine.h"
#include "witch_collisions.h"

#define COL_MALLOC malloc
#define COL_MALLOC_TYPE(type) ((type *)malloc(sizeof(type)))
#define COL_MALLOC_ARR(type, count) ((type *)malloc(sizeof(type) * (count)))
#define COL_CALLOC calloc
#define COL_CALLOC_TYPE(type, count) ((type *)calloc(sizeof(type), (count)))
#define COL_FREE free

void initCollisionMap(const char *image_path, Rectangle frame, uint32_t frames_count, FrameCollisionMap *colMap) {
    Image image = smeLoadImage(image_path);
    Color *pixels = smeLoadImageColors(image);
    smeUnloadImage(image);

    uint32_t imgW = (uint32_t)image.width;
    uint32_t imgH = (uint32_t)image.height;

    uint8_t *imageMask = COL_MALLOC_ARR(uint8_t, imgW * imgH);
    for (uint32_t i = 0; i < imgW * imgH; i++) {
        imageMask[i] = pixels[i].a > 0;
    }

    smeUnloadImageColors(pixels);

    colMap->width = (uint32_t)frame.width;
    colMap->height = (uint32_t)frame.height;
    colMap->count = frames_count;

    uint32_t framePixelsCount = colMap->width * colMap->height;
    colMap->frameMaskList = COL_MALLOC_ARR(uint8_t, framePixelsCount * frames_count);
    for (uint32_t f = 0; f < frames_count; f++) {
        for (uint32_t y = 0; y < colMap->height; y++) {
            for (uint32_t x = 0; x < colMap->width; x++) {
                uint32_t srcX = (uint32_t)frame.x + (f * colMap->width) % imgW + x;
                uint32_t srcY = (uint32_t)frame.y + ((f * colMap->width) / imgW) * colMap->height + y;
                uint8_t mask = 0;
                if (srcX < imgW && srcY < imgH) {
                    mask = imageMask[srcY * imgW + srcX];
                }
                uint32_t dstIndex = f * framePixelsCount + y * colMap->width + x;
                colMap->frameMaskList[dstIndex] = mask;
            }
        }
    }

    COL_FREE(imageMask);
}

uint8_t checkCollision(
    const FrameCollisionMap *aCol, uint32_t aFrameIndex, float ax, float ay,
    const FrameCollisionMap *bCol, uint32_t bFrameIndex, float bx, float by
) {
    if (!aCol || !bCol) return 0;
    if (aFrameIndex >= aCol->count) return 0;
    if (bFrameIndex >= bCol->count) return 0;

    int aLeft = (int)ax;
    int aTop = (int)ay;
    int aRight = aLeft + (int)aCol->width;
    int aBottom = aTop + (int)aCol->height;

    int bLeft = (int)bx;
    int bTop = (int)by;
    int bRight = bLeft + (int)bCol->width;
    int bBottom = bTop + (int)bCol->height;

    if (!(aLeft < bRight && aRight > bLeft && aTop < bBottom && aBottom > bTop)) return 0;

    int cx1 = aLeft > bLeft ? aLeft : bLeft;
    int cy1 = aTop > bTop ? aTop : bTop;
    int cx2 = aRight < bRight ? aRight : bRight;
    int cy2 = aBottom < bBottom ? aBottom : bBottom;

    uint32_t framePixelsA = aCol->width * aCol->height;
    uint32_t framePixelsB = bCol->width * bCol->height;
    uint32_t baseA = aFrameIndex * framePixelsA;
    uint32_t baseB = bFrameIndex * framePixelsB;

    for (int y = cy1; y < cy2; y++) {
        for (int x = cx1; x < cx2; x++) {
            uint32_t axLocal = (uint32_t)(x - aLeft);
            uint32_t ayLocal = (uint32_t)(y - aTop);
            uint32_t bxLocal = (uint32_t)(x - bLeft);
            uint32_t byLocal = (uint32_t)(y - bTop);

            uint32_t idxA = baseA + ayLocal * aCol->width + axLocal;
            uint32_t idxB = baseB + byLocal * bCol->width + bxLocal;

            if (aCol->frameMaskList[idxA] && bCol->frameMaskList[idxB]) {
                return 1;
            }
        }
    }

    return 0;
}
