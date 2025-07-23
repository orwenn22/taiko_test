//
// Created by orwenn on 7/23/25.
//

#include "TaikoRulesetPsp.h"

#include <pspctrl.h>
#include <pspgum.h>

#include "../../Beatmap/TaikoBeatmap/TaikoBeatmap.h"
#include "../../Beatmap/TaikoBeatmap/TaikoEffectPoint.h"
#include "../../Beatmap/TaikoBeatmap/TaikoHit.h"
#include "../../BeatmapPlayer/BeatmapPlayer.h"
#include "../../Platform/Psp/Core.h"
#include "../../Platform/Psp/Graphics/2d.h"
#include "../../Platform/Psp/Graphics/Texture.h"
#include "../../Platform/Psp/Graphics/Vertex.h"

#define PLAYFIELD_WIDTH SCREEN_WIDTH
#define PLAYFIELD_HEIGHT 64.f
#define PLAYFIELD_ORIGIN_Y ((SCREEN_HEIGHT - PLAYFIELD_HEIGHT)/2.f)
#define PLAYFIELD_CENTER_Y (SCREEN_HEIGHT/2.f)
#define RECEPTOR_CENTER_X (PLAYFIELD_HEIGHT*1.5f)
#define OBJECT_TRAVEL_DISTANCE (SCREEN_WIDTH-PLAYFIELD_HEIGHT)

static RulesetInput s_taiko_inputs_psp[] = {
    {PSP_CTRL_LTRIGGER, Side1},
    {PSP_CTRL_RIGHT, Middle1},
    {PSP_CTRL_SQUARE, Middle2},
    {PSP_CTRL_RTRIGGER, Side2}
};
static const int s_taiko_input_count = sizeof(s_taiko_inputs_psp) / sizeof(RulesetInput);

TaikoRulesetPsp::TaikoRulesetPsp() : TaikoRuleset(s_taiko_inputs_psp, s_taiko_input_count), m_taiko_sheet(nullptr), m_keys_opacity{} {
}

TaikoRulesetPsp::~TaikoRulesetPsp() {
    delete m_taiko_sheet;
}

void TaikoRulesetPsp::LoadResources() {
    TaikoRuleset::LoadResources();
    m_taiko_sheet = Texture::Load("res/skin/Taiko/sheet.png");
    m_taiko_sheet->Swizzle();
}

void TaikoRulesetPsp::StartAudio(float offset) {
    TaikoRuleset::StartAudio(offset);
}

void TaikoRulesetPsp::OnGameEnd() {
    TaikoRuleset::OnGameEnd();
}

void TaikoRulesetPsp::HandleInput(const RulesetInputMessage &message) {
    TaikoRuleset::HandleInput(message);

    //Display the key that has just been pressed
    if (message.action >= Middle1 && message.action <= Side2) {
        m_keys_opacity[message.action-Middle1] = 1.f;
    }
}

void TaikoRulesetPsp::Update(float dt) {
    TaikoRuleset::Update(dt);

    for (int i = 0; i < 4; ++i) m_keys_opacity[i] -= dt*5.f;
}

void TaikoRulesetPsp::Draw() {
    Begin2D({0.f, 0.f});

    int current_time = (int)(GetPlayer()->GetTime() * 1000.f);

    DrawPlayfield();
    DrawLeftPart();
    DrawReceptor();
    DrawBarlines(current_time);
    DrawHits(current_time);

    End2D();
}


//////////////////////////
///PRIVATE

void TaikoRulesetPsp::DrawPlayfield() {
    Vertex_Color32_XYZ32 *playfield_vertices = (Vertex_Color32_XYZ32 *) sceGuGetMemory(sizeof(Vertex_Color32_XYZ32) * 4);
    playfield_vertices[0] = {0xFF0a0a0a, 0, PLAYFIELD_ORIGIN_Y, -100.f}; //top left
    playfield_vertices[1] = {0xFF0a0a0a, PLAYFIELD_WIDTH, PLAYFIELD_ORIGIN_Y, -100.f}; //top right
    playfield_vertices[2] = {0xFF0a0a0a, 0, PLAYFIELD_ORIGIN_Y+PLAYFIELD_HEIGHT, -100.f}; //bottom left
    playfield_vertices[3] = {0xFF0a0a0a, PLAYFIELD_WIDTH, PLAYFIELD_ORIGIN_Y+PLAYFIELD_HEIGHT, -100.f}; //bottom right

    uint8_t *playfield_indices = (uint8_t *) sceGuGetMemory(sizeof(uint8_t) * 6);
    playfield_indices[0] = 0;
    playfield_indices[1] = 1;
    playfield_indices[2] = 2;
    playfield_indices[3] = 1;
    playfield_indices[4] = 3;
    playfield_indices[5] = 2;

    sceGumDrawArray(GU_TRIANGLES, Vertex_Color32_XYZ32::flags | GU_INDEX_8BIT | GU_TRANSFORM_3D, 6, playfield_indices, playfield_vertices);
}


void TaikoRulesetPsp::DrawLeftPart() {
    Vertex_UV32_XYZ32 *left_part_vertices = (Vertex_UV32_XYZ32 *) sceGuGetMemory(sizeof(Vertex_UV32_XYZ32) * 4);
    left_part_vertices[0] = {128, 64, 0, PLAYFIELD_ORIGIN_Y, -100.f}; //top left
    left_part_vertices[1] = {192, 64, PLAYFIELD_HEIGHT, PLAYFIELD_ORIGIN_Y, -100.f}; //top right
    left_part_vertices[2] = {128, 128, 0, PLAYFIELD_ORIGIN_Y+PLAYFIELD_HEIGHT, -100.f}; //bottom left
    left_part_vertices[3] = {192, 128, PLAYFIELD_HEIGHT, PLAYFIELD_ORIGIN_Y+PLAYFIELD_HEIGHT, -100.f}; //bottom right

    uint8_t *left_part_indices = (uint8_t *) sceGuGetMemory(sizeof(uint8_t) * 6);
    left_part_indices[0] = 0;
    left_part_indices[1] = 1;
    left_part_indices[2] = 2;
    left_part_indices[3] = 1;
    left_part_indices[4] = 3;
    left_part_indices[5] = 2;

    sceGuColor(0xFFFFFFFF);
    GpuUseTexture(m_taiko_sheet);
    sceGuEnable(GU_TEXTURE_2D);
    sceGumDrawArray(GU_TRIANGLES, Vertex_UV32_XYZ32::flags | GU_INDEX_8BIT | GU_TRANSFORM_3D, 6, left_part_indices, left_part_vertices);

    for (int i = 0; i < 4; ++i) {
        if (m_keys_opacity[i] <= 0.f) continue;

        Vertex_UV32_XYZ32 *key_display_vertices = (Vertex_UV32_XYZ32 *) sceGuGetMemory(sizeof(Vertex_UV32_XYZ32) * 4);
        key_display_vertices[0] = {64.f*i,        128, 0, PLAYFIELD_ORIGIN_Y, -100.f}; //top left
        key_display_vertices[1] = {64.f*i + 64.f, 128, PLAYFIELD_HEIGHT, PLAYFIELD_ORIGIN_Y, -100.f}; //top right
        key_display_vertices[2] = {64.f*i,        192, 0, PLAYFIELD_ORIGIN_Y+PLAYFIELD_HEIGHT, -100.f}; //bottom left
        key_display_vertices[3] = {64.f*i + 64.f, 192, PLAYFIELD_HEIGHT, PLAYFIELD_ORIGIN_Y+PLAYFIELD_HEIGHT, -100.f}; //bottom right

        uint8_t *key_display_indices = (uint8_t *) sceGuGetMemory(sizeof(uint8_t) * 6);
        key_display_indices[0] = 0;
        key_display_indices[1] = 1;
        key_display_indices[2] = 2;
        key_display_indices[3] = 1;
        key_display_indices[4] = 3;
        key_display_indices[5] = 2;

        uint8_t alpha = (uint8_t)(255.f*m_keys_opacity[i]);
        sceGuColor(alpha<<24 | 0xFFFFFF);
        sceGumDrawArray(GU_TRIANGLES, Vertex_UV32_XYZ32::flags | GU_INDEX_8BIT | GU_TRANSFORM_3D, 6, key_display_indices, key_display_vertices);
    }

    sceGuColor(0xFFFFFFFF);
    sceGuDisable(GU_TEXTURE_2D);
}


void TaikoRulesetPsp::DrawReceptor() {
    float size = 45.f;
    float x = RECEPTOR_CENTER_X - size/2.f;
    float y = PLAYFIELD_CENTER_Y - size/2.f;

    Vertex_UV32_XYZ32 *receptor_vertices = (Vertex_UV32_XYZ32 *) sceGuGetMemory(sizeof(Vertex_UV32_XYZ32) * 4);
    uint8_t *receptor_indices = (uint8_t *) sceGuGetMemory(sizeof(uint8_t) * 6);

    receptor_vertices[0] = {0, 0, x, y, -100.f}; //top left
    receptor_vertices[1] = {64, 0, x+size, y, -100.f}; //top right
    receptor_vertices[2] = {0, 64, x, y+size, -100.f}; //bottom left
    receptor_vertices[3] = {64, 64, x+size, y+size, -100.f}; //bottom right
    receptor_indices[0] = 0;
    receptor_indices[1] = 1;
    receptor_indices[2] = 2;
    receptor_indices[3] = 1;
    receptor_indices[4] = 3;
    receptor_indices[5] = 2;

    sceGuColor(0xFFFFFFFF);
    sceGuEnable(GU_TEXTURE_2D);
    sceGumDrawArray(GU_TRIANGLES, Vertex_UV32_XYZ32::flags | GU_INDEX_8BIT | GU_TRANSFORM_3D, 6, receptor_indices, receptor_vertices);
    sceGuDisable(GU_TEXTURE_2D);
}


void TaikoRulesetPsp::DrawBarlines(int current_time) {
    TaikoBeatmap *beatmap = GetBeatmap<TaikoBeatmap>();

    const int line_batch_count = 10; //the amount of barlines we batch together
    int current_effect_point = m_first_effect_point; //the current effect point (barline) we are drawing
    bool keep_looping = true;
    int out_count = 0;
    while (keep_looping) {
        //Allocate space for batching
        Vertex_Color32_XYZ32 *lines_vertices = (Vertex_Color32_XYZ32 *) sceGuGetMemory(sizeof(Vertex_Color32_XYZ32) * 2 * line_batch_count);

        //Fill the array with the 10 next visible effect points
        int i = 0;
        while (i < line_batch_count && current_effect_point < beatmap->m_effect_point_count) {
            if (GetBarlineVertices(current_time, &beatmap->m_effect_points[current_effect_point], &lines_vertices[i*2])) {
                //If the current effect point (barline) is visible, it vertices has been passed in the array
                ++i; //go to next location in array
                out_count = 0;
            }
            else {
                ++out_count;
            }

            //stop drawing barlines if 10 consecutive are offscreen
            if (out_count >= 10) {
                keep_looping = false;
                break;
            }

            ++current_effect_point;
        }

        //Fill remaining vertices with zeroes
        while (i < line_batch_count) {
            lines_vertices[i*2] = {};
            lines_vertices[i*2+1] = {};
            ++i;
        }

        //If we reached the end of the map stop looping
        if (current_effect_point >= beatmap->m_effect_point_count) {
            keep_looping = false;
        }

        sceGumDrawArray(GU_LINES, Vertex_Color32_XYZ32::flags | GU_TRANSFORM_3D, 2 * line_batch_count, nullptr, lines_vertices);
    }
}

bool TaikoRulesetPsp::GetBarlineVertices(int current_time, TaikoEffectPoint *effect_point, Vertex_Color32_XYZ32 *vertices_out) {
    float normalized_position = TimeToPosition(effect_point->time, current_time);
    if (normalized_position >= 1.f) return false;

    float x = RECEPTOR_CENTER_X + normalized_position*OBJECT_TRAVEL_DISTANCE;

    vertices_out[0] = {0xFFEEEEEE, x, PLAYFIELD_ORIGIN_Y, -100.f};
    vertices_out[1] = {0xFFEEEEEE, x, PLAYFIELD_ORIGIN_Y+PLAYFIELD_HEIGHT, -100.f};
    return true;
}


void TaikoRulesetPsp::DrawHits(int current_time) {
    TaikoBeatmap *beatmap = GetBeatmap<TaikoBeatmap>();

    const int hit_batch_count = 10; //the amount of hits we batch together
    int current_hit = m_first_hit_index; //the current hit we are drawing
    bool keep_looping = true;

    sceGuEnable(GU_TEXTURE_2D);
    while (keep_looping) {
        //Allocate space for batching
        Vertex_UV32_Color32_XYZ32 *hit_vertices = (Vertex_UV32_Color32_XYZ32 *) sceGuGetMemory(sizeof(Vertex_UV32_Color32_XYZ32) * 8 * hit_batch_count);
        uint8_t *hit_indices = (uint8_t *) sceGuGetMemory(sizeof(uint8_t) * 12 * hit_batch_count); //switch this to uint16_t if batch count above 20ish

        //Fill the array with the 10 next visible effect points
        int i = 0;
        while (i < hit_batch_count && current_hit < beatmap->m_hit_count) {
            if (GetHitVertices(current_time, &beatmap->m_hits[current_hit], &hit_vertices[i*8], &hit_indices[i*12])) {
                for (int j = 0; j < 12; ++j) hit_indices[i*12 + j] += i * 8;  //adjust indices to match this hit's vertex base

                //If the current hit is visible, it vertices has been passed in the array
                ++i; //go to next location in array
            }

            ++current_hit;
        }

        //Fill remaining vertices with zeroes
        while (i < hit_batch_count) {
            for (int j = 0; j < 8; ++j) hit_vertices[i*8+j] = {}; //fill vertices with blank (might be unnecessary (only [i*8]?))
            for (int j = 0; j < 12; ++j) hit_indices[i*12+j] = i*8; //fill indices to reffer to the blank vertices
            ++i;
        }

        //If we reached the end of the map stop looping
        if (current_hit >= beatmap->m_hit_count) {
            keep_looping = false;
        }

        sceGumDrawArray(GU_TRIANGLES, Vertex_UV32_Color32_XYZ32::flags | GU_INDEX_8BIT | GU_TRANSFORM_3D, 12 * hit_batch_count, hit_indices, hit_vertices);
    }

    sceGuDisable(GU_TEXTURE_2D);
}

bool TaikoRulesetPsp::GetHitVertices(int current_time, TaikoHit *taiko_hit, Vertex_UV32_Color32_XYZ32 *vertices_out, uint8_t *indices_out) {
    float normalized_position = TimeToPosition(taiko_hit->time, current_time);
    if (normalized_position >= 1.f) return false;

    float size = taiko_hit->IsStrong() ? 60.f : 45.f;
    uint32_t color = taiko_hit->IsBlue() ? 0xFFFF0000 : 0xFF0000FF;

    //x and y position of the top left of the hit
    float x = RECEPTOR_CENTER_X + normalized_position*OBJECT_TRAVEL_DISTANCE - size/2.f;
    float y = PLAYFIELD_CENTER_Y - size/2.f;

    //We want hits that needs to be hit first to be on top
    float z = 5.f - (float)(taiko_hit->time - current_time)/1000.f;

    //colored piece
    vertices_out[0] = {64, 0, color, x, y, z}; //top left
    vertices_out[1] = {128, 0, color, x+size, y, z}; //top right
    vertices_out[2] = {64, 64, color, x, y+size, z}; //bottom left
    vertices_out[3] = {128, 64, color, x+size, y+size, z}; //bottom right
    indices_out[0] = 0;
    indices_out[1] = 1;
    indices_out[2] = 2;
    indices_out[3] = 1;
    indices_out[4] = 3;
    indices_out[5] = 2;

    //outline
    vertices_out[4] = {128, 0, 0xFFFFFFFF, x, y, z}; //top left
    vertices_out[5] = {192, 0, 0xFFFFFFFF, x+size, y, z}; //top right
    vertices_out[6] = {128, 64, 0xFFFFFFFF, x, y+size, z}; //bottom left
    vertices_out[7] = {192, 64, 0xFFFFFFFF, x+size, y+size, z}; //bottom right
    indices_out[6] = 4;
    indices_out[7] = 5;
    indices_out[8] = 6;
    indices_out[9] = 5;
    indices_out[10] = 7;
    indices_out[11] = 6;

    return true;
}
