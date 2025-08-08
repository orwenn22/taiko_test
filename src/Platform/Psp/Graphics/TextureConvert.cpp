#include "TextureConvert.h"

bool convert_argb8888_to_abgr4444(int w, int h, uint32_t *src, uint16_t *dest) {
    if (src == nullptr || dest == nullptr || w <= 0 || h <= 0) return false;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            uint32_t abgr8888 = src[x + y*w];

            uint8_t a = ((abgr8888 >> 24) & 0xFF) / 16;
            uint8_t b = ((abgr8888 >> 16) & 0xFF) / 16;
            uint8_t g = ((abgr8888 >> 8) & 0xFF) / 16;
            uint8_t r = (abgr8888 & 0xFF) / 16;

            dest[x + y*w] = ((a << 12) | (b << 8) | (g << 4) | r);
        }
    }

    return true;
}