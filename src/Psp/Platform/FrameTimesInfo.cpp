#include "FrameTimesInfo.h"

#include <pspgum.h>

#include "Graphics/Vertex.h"


//note: 1ms is 1000 ticks
void DrawFrameTimesInfo(const FrameTimesInfo &frame_times_info, Vector2<float> pos, float z, float width) {
    constexpr u64 optimal_fps_ticks = (u64)(1.f/60.f * 1000000.f);

    u64 update_end_time =   frame_times_info.frame_start_draw      - frame_times_info.frame_start;
    u64 draw_end_time =     frame_times_info.frame_end_draw        - frame_times_info.frame_start;
    u64 gpu_exec_end_time = frame_times_info.frame_end_gpu_execute - frame_times_info.frame_start;
    u64 frame_end_time =    frame_times_info.frame_end             - frame_times_info.frame_start;

    u64 u64width = (u64)width;
    float update_end_relative_x   = (float)(update_end_time*u64width   / optimal_fps_ticks);
    float draw_end_relative_x     = (float)(draw_end_time*u64width     / optimal_fps_ticks);
    float gpu_exec_end_relative_x = (float)(gpu_exec_end_time*u64width / optimal_fps_ticks);
    float frame_end_relative_x    = (float)(frame_end_time*u64width    / optimal_fps_ticks);


    Vertex_Color32_XYZ32 *overlay_vertices = (Vertex_Color32_XYZ32 *) sceGuGetMemory(sizeof(Vertex_Color32_XYZ32) * 4 * 4);
    uint8_t *overlay_indices = (uint8_t *) sceGuGetMemory(sizeof(Vertex_Color32_XYZ32) * 6 * 4);

    //waiting for vsync
    Vertex_Color32_XYZ32 *wait_rect = (Vertex_Color32_XYZ32 *) sceGuGetMemory(sizeof(Vertex_Color32_XYZ32) * 4);
    uint8_t *wait_rect_indices = (uint8_t *) sceGuGetMemory(sizeof(uint8_t) * 6);
    overlay_vertices[0] = {0xFFFF4444, pos.x                     , pos.y     , z}; //top left
    overlay_vertices[1] = {0xFFFF4444, pos.x+frame_end_relative_x, pos.y     , z}; //top right
    overlay_vertices[2] = {0xFFFF4444, pos.x                     , pos.y+4, z}; //bottom left
    overlay_vertices[3] = {0xFFFF4444, pos.x+frame_end_relative_x, pos.y+4, z}; //bottom right
    overlay_indices[0] = 0;
    overlay_indices[1] = 1;
    overlay_indices[2] = 2;
    overlay_indices[3] = 1;
    overlay_indices[4] = 3;
    overlay_indices[5] = 2;

    //gpu execution (orange)
    overlay_vertices[4] = {0xFF42c2f5, pos.x                        , pos.y     , z}; //top left
    overlay_vertices[5] = {0xFF42c2f5, pos.x+gpu_exec_end_relative_x, pos.y     , z}; //top right
    overlay_vertices[6] = {0xFF42c2f5, pos.x                        , pos.y+4, z}; //bottom left
    overlay_vertices[7] = {0xFF42c2f5, pos.x+gpu_exec_end_relative_x, pos.y+4, z}; //bottom right
    overlay_indices[6] = 4;
    overlay_indices[7] = 5;
    overlay_indices[8] = 6;
    overlay_indices[9] = 5;
    overlay_indices[10] = 7;
    overlay_indices[11] = 6;

    //draw (red)
    overlay_vertices[8]  = {0xFF4444FF, pos.x                    , pos.y     , z}; //top left
    overlay_vertices[9]  = {0xFF4444FF, pos.x+draw_end_relative_x, pos.y     , z}; //top right
    overlay_vertices[10] = {0xFF4444FF, pos.x                    , pos.y+4, z}; //bottom left
    overlay_vertices[11] = {0xFF4444FF, pos.x+draw_end_relative_x, pos.y+4, z}; //bottom right
    overlay_indices[12] = 8;
    overlay_indices[13] = 9;
    overlay_indices[14] = 10;
    overlay_indices[15] = 9;
    overlay_indices[16] = 11;
    overlay_indices[17] = 10;

    //update (green)
    overlay_vertices[12] = {0xFF44FF44, pos.x                      , pos.y     , z}; //top left
    overlay_vertices[13] = {0xFF44FF44, pos.x+update_end_relative_x, pos.y     , z}; //top right
    overlay_vertices[14] = {0xFF44FF44, pos.x                      , pos.y+4, z}; //bottom left
    overlay_vertices[15] = {0xFF44FF44, pos.x+update_end_relative_x, pos.y+4, z}; //bottom right
    overlay_indices[18] = 12;
    overlay_indices[19] = 13;
    overlay_indices[20] = 14;
    overlay_indices[21] = 13;
    overlay_indices[22] = 15;

    overlay_indices[23] = 14;
    sceGumDrawArray(GU_TRIANGLES, Vertex_Color32_XYZ32::flags | GU_INDEX_8BIT | GU_TRANSFORM_3D, 6*4, overlay_indices, overlay_vertices);

    //red line
    Vertex_Color32_XYZ32 *limit_vertices = (Vertex_Color32_XYZ32 *) sceGuGetMemory(sizeof(Vertex_Color32_XYZ32) * 2);
    limit_vertices[0] = {0xFF0000FF, pos.x + width, pos.y-2, z};
    limit_vertices[1] = {0xFF0000FF, pos.x + width, pos.y+6, z};
    sceGumDrawArray(GU_LINES, Vertex_Color32_XYZ32::flags | GU_TRANSFORM_3D, 2, nullptr, limit_vertices);
}
