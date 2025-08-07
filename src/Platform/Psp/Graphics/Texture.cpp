#include "Texture.h"

#include "Swizzle.h"

#include <pspgu.h>
#include <psputils.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


//used for minimum texture width
static unsigned int NextPowerOfTwo(unsigned int x) {
    if (x == 0) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

static bool IsPowerOfTwo(unsigned int x) {
    //somehow all those computer science classes about binary weren't useless
    return x != 0 && (x & (x - 1)) == 0;
}


Texture::Texture(int w, int h, int pixel_format, uint32_t *data)
    : w(w), h(h), fake_h((int)NextPowerOfTwo(h)),
      pixel_format(pixel_format), data(data),
      minimizing_filter(GU_NEAREST), magnifying_filter(GU_NEAREST),
      swizzled(false), gpu_ready(IsPowerOfTwo(w)),
      status(STATUS_UNKNOWN),
      tex_scale_multiplier_x(1.f/(float)w), tex_scale_multiplier_y(1.f/(float)fake_h) {

}

Texture::~Texture() {
    Free();

    //this is probably unnecessary, but it might mitigate use after free mistakes? idk
    w = 0;
    h = 0;
    fake_h = 0;
    pixel_format = 0;
    data = nullptr;
    swizzled = false;
    status = STATUS_UNKNOWN;
    gpu_ready = false;
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

Texture *Texture::Create(int width, int height, bool ensure_valid_width, bool ensure_valid_height) {
    if (width <= 0 || height <= 0) return nullptr;

    if (ensure_valid_width) width = (int)NextPowerOfTwo((unsigned int)width);
    if (ensure_valid_height) height = (int)NextPowerOfTwo((unsigned int)height);

    size_t alloc_size = sizeof(uint32_t) * width * height;
    uint32_t *pixels = (uint32_t *) malloc(alloc_size);
    if (pixels == nullptr) return nullptr;

    for (size_t i = 0; i < width*height; i++) pixels[i] = 0;
    Texture *result = new Texture(width, height, GU_PSM_8888, pixels);
    result->status = STATUS_MALLOC_MANAGED;
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
    uint32_t *swizzled_data = (uint32_t *) malloc(GetDataSize());
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

size_t Texture::GetDataSize() {
    switch (pixel_format) {
        case GU_PSM_8888: return sizeof(uint32_t) * w * h;
        case GU_PSM_4444: return sizeof(uint16_t) * w * h; //TODO: test if this is correct
        default:
            printf("TODO: implement Texture::GetDataSize() for pixel format %d\n", pixel_format);
            return 0;
    }

    //shouldn't get reached
    return 0;
}


uint32_t Texture::GetPixel(int x, int y) {
    if (swizzled || pixel_format != GU_PSM_8888 || data == nullptr) return 0; //TODO: implement this for other pixel formats (and maybe for swizzled textures as well?)
    if (x < 0 || y < 0 || w <= 0 || h <= 0 || x >= w || y >= h) return 0;

    return data[x + y * w];
}

void Texture::SetPixel(int x, int y, uint32_t color) {
    if (swizzled || pixel_format != GU_PSM_8888 || data == nullptr) return; //TODO: implement this for other pixel formats (and maybe for swizzled textures as well?)
    if (x < 0 || y < 0 || w <= 0 || h <= 0 || x >= w || y >= h) return;

    data[x + y * w] = color;
}


Texture *Texture::CopyAndResize(int width, int height, bool ensure_valid_width, bool ensure_valid_height) {
    Texture *result = Create(width, height, ensure_valid_width, ensure_valid_height);
    if (result == nullptr) return nullptr;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int source_x = x * w / width;
            int source_y = y * h / height;

            result->SetPixel(x, y, GetPixel(source_x, source_y));
            //if (x%10 == 0 && y%10 == 0) printf("%i %i (%x) -> %i %i\n", source_x, source_y, data[source_x + source_y * w], x, y);
        }
    }

    sceKernelDcacheWritebackInvalidateAll();
    return result;
}


Texture *Texture::CopyToVram(void *dest) {
    if (!gpu_ready) return nullptr;
    if ((size_t)dest < 0x04000000ul) return nullptr; //bound check (start of vram)
    if (((size_t)dest)%16 != 0) return nullptr; //textures must be 16-bytes alligned

    size_t data_size = GetDataSize();

    if ((size_t)dest + data_size > 0x04200000ul) return nullptr; //bound check (end of vram)

    //TODO: keep track of where we put textures in vram, and prevent overlaps?

    memcpy(dest, data, data_size);
    auto result = new Texture(w, h, pixel_format, (uint32_t *)dest);
    result->swizzled = swizzled;
    result->status = STATUS_VRAM_MANAGED;

    sceKernelDcacheWritebackInvalidateAll();
    return result;

    //sceGuCopyImage(GU_PSM_8888, 0, 0, w*h, 1, w*h, data, 0, 0, w*h, (void *)(0x04000000+557056+557056+557056)); //memcpy call equivalent
}


static Texture *s_current_texture = nullptr;

void GpuUseTexture(Texture *texture) {
    //Only change texture if necessary
    if (s_current_texture == texture || texture == nullptr) return;

    if (!texture->gpu_ready) {
        printf("Trying to draw a texture with an invalid size (%i*%i (%i))", texture->w, texture->h, texture->fake_h);
        return;
    }

    sceGuTexImage(0, texture->w, texture->fake_h, texture->w, texture->data);
    sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA); //GU_TFX_MODULATE seems required for tinting textures, GU_TFX_REPLACE doesn't take vertex colors into account  TODO : parameter to manipulate this ?
    sceGuTexMode(texture->pixel_format, 0, 0, texture->swizzled ? GU_TRUE : GU_FALSE);
    sceGuTexFilter(texture->minimizing_filter,texture->magnifying_filter);
    sceGuTexScale(texture->tex_scale_multiplier_x, texture->tex_scale_multiplier_y);
    sceGuTexOffset(0.f, 0.f);
    s_current_texture = texture;
}
