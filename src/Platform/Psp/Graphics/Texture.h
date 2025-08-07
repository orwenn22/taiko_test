#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstddef>
#include <cstdint>

#include "../Rectangle.h"
#include "../Vector2.h"

enum TextureDataStatus {
    STATUS_UNKNOWN,
    STATUS_STBI_MANAGED,
    STATUS_MALLOC_MANAGED,
    STATUS_VRAM_MANAGED,
};

struct Texture {
    //Officially, sceGuTexImage (and the psp gpu) only support textures of sizes of multiple of two.
    //However, for the height, we can actually go around that limitation my passing the next multiple of two above the
    //actual height of the texture. This is what fake_h is (if h is a power of two then fake_h = h)
    int w, h, fake_h;
    int pixel_format;
    uint32_t *data;
    int minimizing_filter, magnifying_filter;
    bool swizzled;

    //This is set to true when the width of the texture is a power of two. fake_h is always a power of two therefore
    //the actual height of the texture is irrelevent
    bool gpu_ready;

    //This is used to know which function to call to deallocate data
    TextureDataStatus status;

    //These are used by GpuUseTexture and are passed to the sceGuTexScale function.
    //By default, these are set to 1/w and 1/h in order to have more intuitive
    //UV coordinate (texels instead of values between 0 and 1).
    //If for some reason we don't want this behavior, these can just be changed to 1 and 1.
    //(tex_scale_multiplier_y actually use fake_h instead of h)
    float tex_scale_multiplier_x, tex_scale_multiplier_y;

    ~Texture();

    //Load a texture from an image file (won't readjust the size to be powers of two)
    static Texture *Load(const char *path);

    //Create a blank ABGR8888 texture of the specified width and height.
    //If ensure_valid_width is set to true, the width will be adjusted to the power of two above the specified width
    //(if it isn't one already)
    //Same for the height (however this isn't necessary for using the texture with the gpu due to fake_h, only disable
    //if you know what you are doing though)
    static Texture *Create(int width, int height, bool ensure_valid_width = true, bool ensure_valid_height = true);

    static Texture *Sample(int width, int height);


    void Swizzle();


    //Unload texture data
    void Free();

    //Get allocated texture data size
    size_t GetDataSize();


    //These only work for non-swizzled abgr8888 textures for now
    uint32_t GetPixel(int x, int y); //returns blank if out of bound
    void SetPixel(int x, int y, uint32_t color);


    Texture *CopyAndResize(int width, int height, bool ensure_valid_width = true, bool ensure_valid_height = true);

    //Assuming a setup with fb0/fb1 ABRR8888 and a depth buffer, the first block of available vram
    //starts at 0x04154000  (0x04000000+0d557056+0d557056+0d557056/2)
    //vram starts at 0x04000000 and ends at 0x04200000 (2 MB)
    //(will only allow copying textures with a power of two width)
    Texture *CopyToVram(void *dest);

private:
    Texture(int w, int h, int pixel_format, uint32_t *data);
};

Texture *LoadTexture(const char *path);

void GpuUseTexture(Texture *texture);

#endif //TEXTURE_H
