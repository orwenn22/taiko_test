#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>

#include "../Rectangle.h"
#include "../Vector2.h"

enum TextureDataStatus {
    STATUS_UNKNOWN,
    STATUS_STBI_MANAGED,
    STATUS_MALLOC_MANAGED,
};

// TODO : keep track of color format?

struct Texture {
    int w, h;
    int pixel_format;
    uint32_t *data;
    bool swizzled;

    //This is used to know which function to call to deallocate data
    TextureDataStatus status;

    //These are used by GpuUseTexture and are passed to the sceGuTexScale function.
    //By default, these are set to 1/w and 1/h in order to have more intuitive
    //UV coordinate (texels instead of values between 0 and 1).
    //In for some reason we don't want this behavior, these can just be changed to 1 and 1.
    float tex_scale_multiplier_x, tex_scale_multiplier_y;

    ~Texture();

    static Texture *Load(const char *path);
    static Texture *Sample(int width, int height);

    void Swizzle();
    void Free();

private:
    Texture(int w, int h, int pixel_format, uint32_t *data);
};

Texture *LoadTexture(const char *path);

void GpuUseTexture(Texture *texture);

#endif //TEXTURE_H
