//
// Created by orwenn on 7/23/25.
//

#include "TaikoRulesetPsp.h"

#include <pspctrl.h>
#include <pspgum.h>

#include "../../Platform/Psp/Core.h"
#include "../../Platform/Psp/Graphics/2d.h"
#include "../../Platform/Psp/Graphics/Texture.h"
#include "../../Platform/Psp/Graphics/Vertex.h"

#define PLAYFIELD_WIDTH SCREEN_WIDTH
#define PLAYFIELD_HEIGHT 64.f
#define PLAYFIELD_ORIGIN_Y ((SCREEN_HEIGHT - PLAYFIELD_HEIGHT)/2.f)
#define PLAYFIELD_CENTER_Y (SCREEN_HEIGHT/2.f)

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

    DrawPlayfield();
    DrawLeftPart();



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
