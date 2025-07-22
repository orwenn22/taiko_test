#include "Texture.h"

#include "Swizzle.h"

#include <psputils.h>

#define STB_IMAGE_IMPLEMENTATION
#include <pspgu.h>
#include <stb_image.h>



Texture::Texture(int w, int h, int pixel_format, uint32_t *data) :
w(w), h(h), pixel_format(pixel_format), data(data), swizzled(false), status(STATUS_UNKNOWN),
tex_scale_multiplier_x(1.f/(float)w), tex_scale_multiplier_y(1.f/(float)h) {

}

Texture::~Texture() {
    Free();

    //this is probably unnecessary, but it might mitigate use after free mistakes? idk
    w = 0;
    h = 0;
    pixel_format = 0;
    data = nullptr;
    swizzled = false;
    status = STATUS_UNKNOWN;
}

Texture *Texture::Load(const char *path) {
    if (path == nullptr) return nullptr;

    // try loading image
    int w = 0, h = 0;
    uint32_t *data = (uint32_t *) stbi_load(path, &w, &h, nullptr, STBI_rgb_alpha);

    if (data == nullptr || w == 0 || h == 0) return nullptr;

    sceKernelDcacheWritebackInvalidateAll();
    Texture *result = new Texture(w, h, GU_PSM_8888, data);
    result->status = STATUS_STBI_MANAGED;
    return result;
}

Texture *Texture::Sample(int width, int height) {
    uint32_t *data = (uint32_t *) malloc(sizeof(uint32_t) * width * height);
    if (data == nullptr) return nullptr;

    int gradiant_factor_x = 255 / width;
    int gradiant_factor_y = 255 / height;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // make a cool gradiant idk
            //                     A              B                               G                            R
            data[x + y * width] = 0xFF000000 | ((x*gradiant_factor_x) << 16) | ((y*gradiant_factor_y) << 8) | 0xFF;

            if (x == 16) data[x + y * width] = 0xFF000000;
            if (y == 0 && x < 18) data[x + y * width] = 0xFF000000;
        }
    }

    Texture *result = new Texture(width, height, GU_PSM_8888, data);
    result->status = STATUS_MALLOC_MANAGED;
    return result;
}

void Texture::Swizzle() {
    if (data == nullptr || w <= 0 || h <= 0 || w%8 != 0 || h%8 != 0) return;
    if (swizzled) return;

    // allocate memory for the swizzled texture
    uint32_t *swizzled_data = (uint32_t *) malloc(sizeof(uint32_t) * w * h);
    if (swizzled_data == nullptr) return;

    // swizzle the texture
    switch (pixel_format) {
        case GU_PSM_8888: swizzle_abgr8888(swizzled_data, data, w, h); break;
        case GU_PSM_4444: swizzle_abgr4444((uint8_t *)swizzled_data, (const uint8_t *)data, w, h); break; // TODO : test if this actually works lmao

        default:
            // TODO : implement relevant swizzle function for this format.
            // just return for now
            free(swizzled_data);
            return;
    }

    // clear the previous data buffer and replace it with the swizzled texture
    Free();
    data = swizzled_data;
    status = STATUS_MALLOC_MANAGED;
    swizzled = true;

    sceKernelDcacheWritebackInvalidateAll();
}

void Texture::Free() {
    if (status == STATUS_STBI_MANAGED) stbi_image_free(data);
    else if (status == STATUS_MALLOC_MANAGED) free(data);
    else return; //maybe we should do something in that case

    status = STATUS_UNKNOWN;
    data = nullptr;
}

static Texture *s_current_texture = nullptr;

void GpuUseTexture(Texture *texture) {
    //Only change texture if necessary
    if (s_current_texture == texture) return;

    sceGuTexImage(0, texture->w, texture->h, texture->w, texture->data);
    sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA); //GU_TFX_MODULATE seems required for tinting textures, GU_TFX_REPLACE doesn't take vertex colors into account  TODO : parameter to manipulate this ?
    sceGuTexMode(texture->pixel_format, 0, 0, texture->swizzled ? GU_TRUE : GU_FALSE);
    sceGuTexFilter(GU_NEAREST,GU_NEAREST); //TODO : parameter to manipulate this?
    sceGuTexScale(texture->tex_scale_multiplier_x, texture->tex_scale_multiplier_y); //TODO: do this only one time somehow (and not evey time we use the texture)
    sceGuTexOffset(0.f, 0.f);
    s_current_texture = texture;
}
