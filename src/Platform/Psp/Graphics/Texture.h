#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>

#include "../Rectangle.h"
#include "../Vector2.h"

enum TextureDataStatus {
    STATUS_UNKNOWN,
    STATUS_STBI_MANAGED,
    STATUS_MALLOC_MANAGED,
    STATUS_VRAM_MANAGED, //TODO: Maybe make some kind of custom allocator for vram?
};

//TODO: We might actually not need the height of the texture to be a power of 2 to make the gpu happy
//      as long as we pass the next power of 2 of the height to sceGuTexImage it should be fine
//      (right now CopyToVram will copy stuff outside of vram if the texture is too big, might be an issue in the future)

struct Texture {
    int w, h;
    int pixel_format;
    uint32_t *data;
    int minimizing_filter, magnifying_filter;
    bool swizzled;
    bool gpu_ready;

    //This is used to know which function to call to deallocate data
    TextureDataStatus status;

    //These are used by GpuUseTexture and are passed to the sceGuTexScale function.
    //By default, these are set to 1/w and 1/h in order to have more intuitive
    //UV coordinate (texels instead of values between 0 and 1).
    //In for some reason we don't want this behavior, these can just be changed to 1 and 1.
    float tex_scale_multiplier_x, tex_scale_multiplier_y;

    ~Texture();

    static Texture *Load(const char *path);
    static Texture *Create(int width, int height, bool ensure_valid_size = true);
    static Texture *Sample(int width, int height);

    void Swizzle();
    void Free();

    //Return the ABGR color at a specific pixel coordinate (returns blank if out of bound)
    uint32_t GetPixel(int x, int y);

    void SetPixel(int x, int y, uint32_t color);

    Texture *CopyAndResize(int width, int height, bool ensure_valid_size = true);
    Texture *CopyToVram(); //TODO: (check cpp file for actual TODO)

private:
    Texture(int w, int h, int pixel_format, uint32_t *data);
};

Texture *LoadTexture(const char *path);

void GpuUseTexture(Texture *texture);

#endif //TEXTURE_H
