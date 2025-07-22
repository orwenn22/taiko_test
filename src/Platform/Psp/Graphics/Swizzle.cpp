#include "Swizzle.h"

#include <cstdint>

#define BLOCK_WIDTH 4
#define BLOCK_HEIGHT 8

// for abgr8888
// self-made by looking at how a non-swizzled texture is displayed with the swizzle flag enabled,
// so it probably has tons of issues
void swizzle_abgr8888(uint32_t *out, const uint32_t *in, unsigned int width, unsigned int height) {
    unsigned int h_block_count = width / BLOCK_WIDTH;
    unsigned int v_block_count = height / BLOCK_HEIGHT;

    int dest_index = 0;

    for (unsigned int v = 0; v < v_block_count; ++v) {
        for (unsigned int h = 0; h < h_block_count; ++h) {
            for (unsigned int y = 0; y < BLOCK_HEIGHT; ++y) {
                for (unsigned int x = 0; x < BLOCK_WIDTH; ++x) {
                    unsigned int source_x = h * BLOCK_WIDTH + x;
                    unsigned int source_y = v * BLOCK_HEIGHT + y;
                    out[dest_index++] = in[source_x + source_y * width];
                }
            }
        }
    }
}


//for abgr4444
//from https://github.com/pspdev/pspsdk/blob/master/src/samples/gu/blit/blit.c therefore it is most likely correct
void swizzle_abgr4444(uint8_t *out, const uint8_t *in, unsigned int width, unsigned int height) {
    unsigned int blockx, blocky;
    unsigned int j;

    unsigned int width_blocks = (width / 16);
    unsigned int height_blocks = (height / 8);

    unsigned int src_pitch = (width-16)/4;
    unsigned int src_row = width * 8;

    const uint8_t* ysrc = in;
    uint32_t* dst = (uint32_t* )out;

    for (blocky = 0; blocky < height_blocks; ++blocky) {
        const uint8_t *xsrc = ysrc;
        for (blockx = 0; blockx < width_blocks; ++blockx) {
            const uint32_t *src = (uint32_t *)xsrc;
            for (j = 0; j < 8; ++j) {
                *(dst++) = *(src++);
                *(dst++) = *(src++);
                *(dst++) = *(src++);
                *(dst++) = *(src++);
                src += src_pitch;
            }
            xsrc += 16;
        }
        ysrc += src_row;
    }
}
