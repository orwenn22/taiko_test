#include "UtilsRaylib.h"

Color ABGR8888ColorToRaylib(uint32_t color) {
    return {
        (unsigned char)(color & 0xff),
        (unsigned char)((color >> 8) & 0xff),
        (unsigned char)((color >> 16) & 0xff),
        (unsigned char)((color >> 24) & 0xff)
    };
}
