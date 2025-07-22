#include "2d.h"

#include <pspgu.h>
#include <pspgum.h>
#include <cstddef>

#include "Font.h"
#include "Texture.h"
#include "Vertex.h"

//NOTE: this assumes sceGuDepthRange(65535, 0);
void Begin2D(const Vector2<float> &origin, const Vector2<float> &offset, float zoom) {
    //This setup makes it so any object with a higher Z index will be drawn above
    ScePspFVector3 eye = {origin.x, origin.y, 100.f};
    ScePspFVector3 center = {origin.x,  origin.y, 0.f};
    ScePspFVector3 up = {0.f, 1.f, 0.f};

    //Look into our 2D world
    sceGumMatrixMode(GU_VIEW);
    sceGumLoadIdentity();
    sceGumLookAt(&eye, &center, &up);

    //Objects will appear with the same size regardless of their Z coordinate.
    sceGumMatrixMode(GU_PROJECTION);
    sceGumLoadIdentity();
    sceGumOrtho(-offset.x/zoom, (480-offset.x)/zoom, (272-offset.y)/zoom, -offset.y/zoom, 0.1f, 300.f);

    //Reset model matrix
    sceGumMatrixMode(GU_MODEL);
    sceGumLoadIdentity();

    //depth buffer setup with sceGuDepthRange(65535, 0);
    sceGuDepthFunc(GU_GEQUAL);
    sceGuClearDepth(0); //0 because depth test function is gequal
    sceGuClear(GU_DEPTH_BUFFER_BIT); //TODO: figure out if we really really REALLY want to clear the depth buffer here

    //using this instead of the above would invert the depth buffer
    ///this should also be used with sceGuDepthRange(0, 65535); instead of the above. probably.
    //sceGuDepthFunc(GU_LEQUAL);
    //sceGuClearDepth(0xffff); //0xffff because depth test function is lequal
    //sceGuClear(GU_DEPTH_BUFFER_BIT);
}

//NOTE: this assumes sceGuDepthRange(65535, 0);
void Begin2DPerspective(const Vector2<float> &target) {
    //This setup makes it so any object with a lower Z index will be drawn above
    ScePspFVector3 eye = {target.x, target.y, -329.6f};
    ScePspFVector3 center = {target.x,  target.y, 0.f};
    ScePspFVector3 up = {0.f, -1.f, 0.f};

    //Look into our 2D world
    sceGumMatrixMode(GU_VIEW);
    sceGumLoadIdentity();
    sceGumLookAt(&eye, &center, &up);

    //Objects that are far away from the camera will be smaller
    sceGumMatrixMode(GU_PROJECTION);
    sceGumLoadIdentity();
    sceGumPerspective(45.f, 16.f/9.f, 1.f, 1000.f);

    sceGumMatrixMode(GU_MODEL);
    sceGumLoadIdentity();

    //depth buffer setup with sceGuDepthRange(65535, 0);
    sceGuDepthFunc(GU_GEQUAL);
    sceGuClearDepth(0); //0 because depth test function is gequal
    sceGuClear(GU_DEPTH_BUFFER_BIT); //TODO: figure out if we really really REALLY want to clear the depth buffer here

    //this works with sceGuDepthRange(0, 65535);
    //sceGuDepthFunc(GU_LEQUAL);
    //sceGuClearDepth(0xffff); //0xffff because depth test function is lequal
    //sceGuClear(GU_DEPTH_BUFFER_BIT);
}

void End2D() {
    Begin2D(Vector2<float>(0, 0));
}


void DrawText(const char *text, Font *font, const Vector2<float> &pos, float z, float scale, unsigned int color) {
    if (text == nullptr) return;
    if (font == nullptr) return;

    GpuUseTexture(font->texture);
    sceGuEnable(GU_TEXTURE_2D); //Use texture to draw

    Vector2<float> pos_offset = pos;

    size_t i = 0;
    bool end_reached = false;

    while (!end_reached) {
        //alloc (draw 5 characters by draw calls)
        Vertex_UV32_Color32_XYZ32 *vertices = (Vertex_UV32_Color32_XYZ32*)sceGuGetMemory(5 * 6 * sizeof(Vertex_UV32_Color32_XYZ32));

        int char_count = 0;
        while (char_count < 5 && !end_reached) {
            char c = text[i];

            if (c == 0) {
                end_reached = true;
                continue;
            }

            if (c == '\n') {
                pos_offset.x = pos.x;
                pos_offset.y += (float)font->char_height * scale;
                ++i;
                continue;
            }

            //change color (\x1\xff\x00\x00 set the text to blue)
            if (c == '\1') {
                unsigned char b = text[i+1];
                unsigned char g = text[i+2];
                unsigned char r = text[i+3];
                color = 0xFF000000 | (b << 16) | (g << 8) | r;
                i += 4;
                continue;
            }

            Rectangle<float> uv_rect = {
                (float)(font->char_width * (c%font->char_count_x)),
                (float)(font->char_height * (c/font->char_count_x)),
                (float)font->char_width,
                (float)font->char_height};

            int top_left_vertex = char_count*6;

            //top left
            vertices[top_left_vertex].x = pos_offset.x;
            vertices[top_left_vertex].y = pos_offset.y;
            vertices[top_left_vertex].z = z;
            vertices[top_left_vertex].u = uv_rect.x;
            vertices[top_left_vertex].v = uv_rect.y;
            vertices[top_left_vertex].color = color;

            //top right
            vertices[top_left_vertex+1].x = pos_offset.x + font->char_width * scale;
            vertices[top_left_vertex+1].y = pos_offset.y;
            vertices[top_left_vertex+1].z = z;
            vertices[top_left_vertex+1].u = uv_rect.x+uv_rect.w;
            vertices[top_left_vertex+1].v = uv_rect.y;
            vertices[top_left_vertex+1].color = color;

            //bottom left
            vertices[top_left_vertex+2].x = pos_offset.x;
            vertices[top_left_vertex+2].y = pos_offset.y + font->char_height * scale;
            vertices[top_left_vertex+2].z = z;
            vertices[top_left_vertex+2].u = uv_rect.x;
            vertices[top_left_vertex+2].v = uv_rect.y+uv_rect.h;
            vertices[top_left_vertex+2].color = color;

            //top right
            vertices[top_left_vertex+3].x = pos_offset.x + font->char_width * scale;
            vertices[top_left_vertex+3].y = pos_offset.y;
            vertices[top_left_vertex+3].z = z;
            vertices[top_left_vertex+3].u = uv_rect.x+uv_rect.w;
            vertices[top_left_vertex+3].v = uv_rect.y;
            vertices[top_left_vertex+3].color = color;

            //bottom right
            vertices[top_left_vertex+4].x = pos_offset.x + font->char_width * scale;
            vertices[top_left_vertex+4].y = pos_offset.y + font->char_height * scale;
            vertices[top_left_vertex+4].z = z;
            vertices[top_left_vertex+4].u = uv_rect.x+uv_rect.w;
            vertices[top_left_vertex+4].v = uv_rect.y+uv_rect.h;
            vertices[top_left_vertex+4].color = color;

            //bottom left
            vertices[top_left_vertex+5].x = pos_offset.x;
            vertices[top_left_vertex+5].y = pos_offset.y + font->char_height * scale;
            vertices[top_left_vertex+5].z = z;
            vertices[top_left_vertex+5].u = uv_rect.x;
            vertices[top_left_vertex+5].v = uv_rect.y+uv_rect.h;
            vertices[top_left_vertex+5].color = color;

            ++char_count;
            ++i;
            pos_offset.x += font->char_width * scale;
        }

        //in the case we reached the end of the string and don't have a group of 5 chars we need to clear the
        //remaining vertices
        while (char_count < 5) {
            for (int i = 0; i < 6; ++i) {
                vertices[char_count*6+i].x = 0;
                vertices[char_count*6+i].y = 0;
                vertices[char_count*6+i].z = 0;
                vertices[char_count*6+i].u = 0;
                vertices[char_count*6+i].v = 0;
                vertices[char_count*6+i].color = 0x00000000;
            }
            ++char_count;
        }
        sceGumDrawArray(GU_TRIANGLES, Vertex_UV32_Color32_XYZ32::flags | GU_TRANSFORM_3D, 5*6, 0, vertices);
    }

    sceGuDisable(GU_TEXTURE_2D);
}
