#ifndef SWIZZLE_H
#define SWIZZLE_H

#include <cstdint>

void swizzle_abgr8888(uint32_t *out, const uint32_t *in, unsigned int width, unsigned int height);
void swizzle_abgr4444(uint16_t *out, const uint16_t *in, unsigned int width, unsigned int height);

#endif //SWIZZLE_H
