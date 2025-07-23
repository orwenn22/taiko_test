#include "Font.h"

#include "Texture.h"

Font::Font(Texture *texture, int char_width, int char_height, bool should_free_texture) : texture(texture), should_free_texture(should_free_texture), char_width(char_width), char_height(char_height) {
    char_count_x = texture->w / char_width;
    char_count_y = texture->h / char_height;
    char_count = char_count_x * char_count_y;
}

Font::~Font() {
    if (should_free_texture) delete texture;
    texture = nullptr;
    should_free_texture = false;
    char_width = 0;
    char_height = 0;
}

Font *Font::FromTexture(Texture *texture, int char_width, int char_height) {
    if (texture == nullptr || char_width <= 0 || char_height <= 0) return nullptr;
    return new Font(texture, char_width, char_height, false);
}

Font *Font::FromPath(const char *path, int char_width, int char_height) {
    if (path == nullptr || char_width <= 0 || char_height <= 0) return nullptr;
    Texture *texture = Texture::Load(path);
    if (texture == nullptr) return nullptr;
    //texture->Swizzle(); //TODO: figure out why this prevents to exit the program properly
    return new Font(texture, char_width, char_height, true);
}
