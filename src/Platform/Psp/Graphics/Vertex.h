#ifndef VERTEX_H
#define VERTEX_H

#include <pspgu.h>

// Data members inside a vertex are laid out in the following order:
// Weights - if GU_WEIGHTS(n) is used N weights are present
// Texture Coordinates
// Color
// Normal
// Position

typedef struct Vertex_Color32_XYZ32 {
    unsigned int color; //requires 'sceGuShadeModel(GU_SMOOTH);' in init, expect GU_COLOR_8888 in sceGumDrawArray
    float x, y, z; //Expect GU_VERTEX_32BITF in sceGumDrawArray

    static constexpr int flags = GU_COLOR_8888 | GU_VERTEX_32BITF;
} Vertex_Color32_XYZ32;

typedef struct Vertex_UV32_XYZ32 {
    float u, v; //expect GU_TEXTURE_32BITF in sceGumDrawArray
    float x, y, z; //Expect GU_VERTEX_32BITF in sceGumDrawArray

    static constexpr int flags = GU_TEXTURE_32BITF | GU_VERTEX_32BITF;
} Vertex_UV32_XYZ32;

typedef struct Vertex_UV32_Color32_XYZ32 {
    float u, v; //expect GU_TEXTURE_32BITF in sceGumDrawArray
    unsigned int color; //requires 'sceGuShadeModel(GU_SMOOTH);' in init, expect GU_COLOR_8888 in sceGumDrawArray
    float x, y, z; //Expect GU_VERTEX_32BITF in sceGumDrawArray

    static constexpr int flags = GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF;
} Vertex_UV32_Color32_XYZ32;

#endif //VERTEX_H
