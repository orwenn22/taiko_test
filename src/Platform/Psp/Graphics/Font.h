#ifndef FONT_H
#define FONT_H


struct Texture;

struct Font {
    Texture *texture;
    bool should_free_texture;
    int char_width, char_height;
    int char_count_x, char_count_y, char_count;

    ~Font();

    static Font *FromTexture(Texture *texture, int char_width, int char_height);
    static Font *FromPath(const char *path, int char_width, int char_height);

private:
    Font(Texture *texture, int char_width, int char_height, bool should_free_texture);
};



#endif //FONT_H
