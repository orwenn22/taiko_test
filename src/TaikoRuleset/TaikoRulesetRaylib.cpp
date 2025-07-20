#include "TaikoRulesetRaylib.h"

#include "raymath.h"
#include "rlgl.h"
#include "../BeatmapPlayer.h"
#include "../TaikoBeatmap/TaikoBeatmap.h"
#include "../TaikoBeatmap/TaikoDrumRoll.h"
#include "../TaikoBeatmap/TaikoEffectPoint.h"
#include "../TaikoBeatmap/TaikoHit.h"


static RulesetInput s_taiko_inputs_raylib[] = {
    {KEY_D, Side1},
    {KEY_F, Middle1},
    {KEY_J, Middle2},
    {KEY_K, Side2}
};
static const int s_taiko_input_count = sizeof(s_taiko_inputs_raylib) / sizeof(RulesetInput);


TaikoRulesetRaylib::TaikoRulesetRaylib()
    : TaikoRuleset(s_taiko_inputs_raylib, s_taiko_input_count),
      m_camera{},
      m_audio{}, m_small_hit_texture{}, m_small_hit_overlay_texture{}, m_drum_roll_tail_texture{}, m_drum_roll_middle_texture{},
      m_shader{},
      m_playfield_width(970.f), m_object_travel_distance(m_playfield_width-128) {

}

TaikoRulesetRaylib::~TaikoRulesetRaylib() {
    UnloadTexture(m_small_hit_texture);
    UnloadTexture(m_small_hit_overlay_texture);
    UnloadTexture(m_receptor_texture);
    UnloadTexture(m_drum_roll_tail_texture);
    UnloadTexture(m_drum_roll_middle_texture);
    UnloadShader(m_shader);
    UnloadMusicStream(m_audio);
}


void TaikoRulesetRaylib::LoadResources() {
    TaikoRuleset::LoadResources();
    m_audio = LoadMusicStream(TextFormat("res/%s", GetBeatmap<TaikoBeatmap>()->m_audio_filename));

    m_small_hit_texture = LoadTexture("res/skin/Taiko/taikohitcircle.png");
    m_small_hit_overlay_texture = LoadTexture("res/skin/Taiko/taikohitcircleoverlay.png");
    m_drum_roll_tail_texture = LoadTexture("res/skin/Taiko/taikodrumrolltail.png");
    m_drum_roll_middle_texture = LoadTexture("res/skin/Taiko/taikodrumrollmiddle.png");
    m_receptor_texture = LoadTexture("res/skin/Taiko/receptor.png");

    m_shader = LoadShader("res/shader/discard_transparent.vs", "res/shader/discard_transparent.fs");
}

void TaikoRulesetRaylib::OnGameStart() {
    TaikoRuleset::OnGameStart();
    PlayMusicStream(m_audio);
}

void TaikoRulesetRaylib::OnGameEnd() {
    TaikoRuleset::OnGameEnd();
    StopMusicStream(m_audio);
}


void TaikoRulesetRaylib::Update(float dt) {
    UpdateMusicStream(m_audio);
    TaikoRuleset::Update(dt);

    //Update camera
    const float screen_width = (float) GetScreenWidth();
    const float screen_height = (float) GetScreenHeight();
    m_camera = {
        .position = (Vector3){ screen_width / 2.0f, screen_height / 2.0f, 100.0f }, // In front of the scene
        .target   = (Vector3){ screen_width / 2.0f, screen_height / 2.0f, 0.0f }, // Looking at the center
        .up       = (Vector3){ 0.0f, 1.0f, 0.0f },
        .fovy     = screen_height,  // Special meaning in ORTHOGRAPHIC mode (Y size of view)
        .projection = CAMERA_ORTHOGRAPHIC
    };

    m_playfield_width = screen_width;
    m_object_travel_distance = m_playfield_width-128;
}

//FIXME: THIS IS SCUFFED AF
//Ok, so, for a while i was wondering how to display the objects in the correct order.
//The approach i took was actually rendering a 3D scene with an orthographic projection, and then draw billboards for
//each elements at the correct z coordinate.
//However, that also meant that we would draw objects below objects that were proviously drawn, and the depth buffer
//will get written even at the locations where the texture is transparent.
//To fix this i made a fragment shader that will ignore pixels that are too transparent.
//All of this probably isn't ideal, but ITS WORKING SOMEHOW
void TaikoRulesetRaylib::Draw() {
    auto beatmap = GetBeatmap<TaikoBeatmap>();

    //current time in ms from the start of the beatmap
    int current_time = (int)(GetPlayer()->GetTime() * 1000.f);

    //TODO: draw playfield in 3D mode?
    DrawRectangle(0, 540/2 - 128/2, (int) m_playfield_width, 128, WHITE);
    DrawRectangle(0, 540/2 - 128/2 + 1, (int) m_playfield_width, 128-2, BLACK);

    BeginMode3D(m_camera);
    BeginShaderMode(m_shader);

    //Old 2D code for receptor
    //DrawCircle(192, 540/2, 37, WHITE);
    //DrawCircle(192, 540/2, 35, GRAY);

    //Hit receptor
    DrawBillboardPro(m_camera,
        m_receptor_texture, {0.f, 0.f, (float) m_receptor_texture.width, (float) m_receptor_texture.height},
        {192, 540.f/2.f, -100},
        {0, 1, 0},
        {85, 85}, {85.f/2.f, 85.f/2.f}, 0,
        WHITE);

    //Draw barlines
    int barline_out_count = 0;
    //printf("%i\n", m_first_effect_point);
    for (int i = m_first_effect_point; i < beatmap->m_effect_point_count; ++i) {
        TaikoEffectPoint *effect_point = &(beatmap->m_effect_points[i]);
        float position = TimeToPosition(effect_point->time, current_time);

        if (position >= 1) {
            ++barline_out_count;
            if (barline_out_count >= 15) break;
            continue;
        }
        barline_out_count = 0;

        DrawLine(192 + position*m_object_travel_distance, 540/2-64, 192 + position*m_object_travel_distance, 540/2+64, GRAY);
    }

    DrawHits(current_time);
    DrawDrumRolls(current_time);

    EndShaderMode();
    EndMode3D();
}


///////////////////////////
/// PRIVATE

void TaikoRulesetRaylib::DrawHits(int current_time) {
    auto beatmap = GetBeatmap<TaikoBeatmap>();

    int out_count = 0;
    for (int i = m_first_hit_index; i < beatmap->m_hit_count; ++i) {
        TaikoHit *hit = &(beatmap->m_hits[i]);

        //We want to stop drawing hits that are outside the view at some point.
        //Idk how to do this correctly, so let's just stop whenever we have 15 consecutive hits outside.
        if (DrawHit(hit, current_time) == false) {
            //++out_count;
            //if (out_count >= 15) break;
            continue;
        }
        //out_count = 0;
    }
}

bool TaikoRulesetRaylib::DrawHit(TaikoHit *hit, int current_time) {
    float normalized_position = TimeToPosition(hit->time, current_time);
    if (normalized_position >= 1) return false;

    //192 is the hit receptor position and 960 is the width of the playfield
    float x = 192 + normalized_position*m_object_travel_distance;
    float size = (hit->IsStrong()) ? 115.f : 80.f;
    float origin = size/2.f;

    //We want hits that needs to be hit first to be on top
    float z = 5.f - (float)(hit->time - current_time)/1000.f;

    //Original 2D shape code
    //DrawCircle(x, 540/2, 35.f + 25.f*hit->IsStrong(), hit->IsBlue() ? BLUE : RED);

    DrawBillboardPro(m_camera,
            m_small_hit_texture, {0.f, 0.f, (float) m_small_hit_texture.width, (float) m_small_hit_texture.height},
            {x, 540.f/2, z},
            {0, 1, 0},
            {size, size}, {origin, origin}, 0,
            hit->IsBlue() ? BLUE : RED);

    DrawBillboardPro(m_camera,
        m_small_hit_overlay_texture, {0.f, 0.f, (float) m_small_hit_overlay_texture.width, (float) m_small_hit_overlay_texture.height},
        {x, 540.f/2, z},
        {0, 1, 0},
        {size, size}, {origin, origin}, 0,
        WHITE);

    return true;
}


void TaikoRulesetRaylib::DrawDrumRolls(int current_time) {
    auto beatmap = GetBeatmap<TaikoBeatmap>();

    int out_count = 0;
    for (int i = m_first_drum_roll; i < beatmap->m_drum_roll_count; ++i) {
        TaikoDrumRoll *drum_roll = &(beatmap->m_drum_rolls[i]);

        if (DrawDrumRoll(drum_roll, current_time) == false) {
            ++out_count;
            if (out_count >= 15) break;
            continue;
        }
        out_count = 0;
    }
}

bool TaikoRulesetRaylib::DrawDrumRoll(TaikoDrumRoll *drum_roll, int current_time) {
    float normalized_position_head = TimeToPosition(drum_roll->time, current_time);
    if (normalized_position_head >= 1) return false;

    //We want to use the same effect point as the head, because the visual length of the drum roll never changed even
    //if there are timing points between them
    float normalized_position_tail = TimeToPosition(drum_roll->GetEndTime(), current_time, drum_roll->time);

    float head_x = 192 + normalized_position_head*m_object_travel_distance;
    float tail_x = 192 + normalized_position_tail*m_object_travel_distance;
    float size = drum_roll->strong ? 115.f : 80.f;
    float origin = size/2.f;
    float z = 5.f - (float)(drum_roll->time - current_time)/1000.f;

    //Tail
    DrawBillboardPro(m_camera,
        m_drum_roll_tail_texture, {0.f, 0.f, (float) m_drum_roll_tail_texture.width, (float) m_drum_roll_tail_texture.height},
        {tail_x, 540.f/2, z},
        {0, 1, 0},
        {size, size}, {origin, origin}, 0,
        { 230, 220, 0, 255 });

    //Middle part
    DrawBillboardPro(m_camera,
        m_drum_roll_middle_texture, {0.f, 0.f, (float) m_drum_roll_middle_texture.width, (float) m_drum_roll_middle_texture.height},
        {head_x, 540.f/2, z},
        {0, 1, 0},
        {tail_x-head_x, size}, {0.f, origin}, 0,
        { 230, 220, 0, 255 });

    //Head
    DrawBillboardPro(m_camera,
            m_small_hit_texture, {0.f, 0.f, (float) m_small_hit_texture.width, (float) m_small_hit_texture.height},
            {head_x, 540.f/2, z},
            {0, 1, 0},
            {size, size}, {origin, origin}, 0,
            { 230, 220, 0, 255 });

    DrawBillboardPro(m_camera,
        m_small_hit_overlay_texture, {0.f, 0.f, (float) m_small_hit_overlay_texture.width, (float) m_small_hit_overlay_texture.height},
        {head_x, 540.f/2, z},
        {0, 1, 0},
        {size, size}, {origin, origin}, 0,
        WHITE);


    return true;
}
