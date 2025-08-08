#ifndef TAIKO_TEST_TEXTURECONVERT_H
#define TAIKO_TEST_TEXTURECONVERT_H

#include <cstdint>

bool convert_argb8888_to_abgr4444(int w, int h, uint32_t *src, uint16_t *dest);

#endif //TAIKO_TEST_TEXTURECONVERT_H