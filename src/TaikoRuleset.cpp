#include "TaikoRuleset.h"

#include <cstdio>

#include "BeatmapPlayer.h"
#include "TaikoBeatmap/TaikoBeatmap.h"

#include "raylib.h"
#include "TaikoBeatmap/TaikoEffectPoint.h"
#include "TaikoBeatmap/TaikoHit.h"

static RulesetRating s_taiko_ratings[] = {
    {"Great", 300, 300, false, true, false, 0xFFFF5050},
    {"Ok", 150, 300, false, true, false, 0xFF50FF50},
    {"Miss", 0, 300, false, false, true, 0xFF0000FF}
};
static const int s_taiko_rating_count = sizeof(s_taiko_ratings) / sizeof(RulesetRating);

static RulesetInput s_taiko_inputs[] = {
    {KEY_D, Side1},
    {KEY_F, Middle1},
    {KEY_J, Middle2},
    {KEY_K, Side2}
};
static const int s_taiko_input_count = sizeof(s_taiko_inputs) / sizeof(RulesetInput);

#define TAIKO_GREAT 0
#define TAIKO_OK 1
#define TAIKO_MISS 2

TaikoRuleset::TaikoRuleset() : Ruleset(s_taiko_ratings, s_taiko_rating_count, s_taiko_inputs, s_taiko_input_count), m_first_hit_index(0), m_first_effect_point(0), m_audio{} {
    ComputeHitWindows();
}

TaikoRuleset::~TaikoRuleset() {
}

void TaikoRuleset::OnBeatmapLoaded() {
    Ruleset::OnBeatmapLoaded();
    ComputeHitWindows();
    GetPlayer()->m_health = 0.f; //Taiko health start at 0
    m_audio = LoadMusicStream(TextFormat("res/%s", GetBeatmap<TaikoBeatmap>()->m_audio_filename));
}

void TaikoRuleset::OnGameStart() {
    Ruleset::OnGameStart();
    PlayMusicStream(m_audio);
}

bool TaikoRuleset::IsOver() {
    TaikoBeatmap *beatmap = GetBeatmap<TaikoBeatmap>();
    if (beatmap == nullptr) return true;
    return (m_first_hit_index >= beatmap->m_hit_count);
}

void TaikoRuleset::OnGameEnd() {
    Ruleset::OnGameEnd();
    StopMusicStream(m_audio);
}

bool TaikoRuleset::ShouldFail() {
    if (IsOver() && GetPlayer()->m_health < 50.f) return true;
    return false;
}

void TaikoRuleset::HandleInput(const RulesetInputMessage &message) {
    if (HandleHit(message)) {
        ++m_first_hit_index;
        return;
    }
}

void TaikoRuleset::Update(float dt) {
    UpdateMusicStream(m_audio);

    auto beatmap = GetBeatmap<TaikoBeatmap>();
    BeatmapPlayer *player = GetPlayer();

    //current time in ms
    int current_time = (int)(GetPlayer()->GetTime() * 1000.f);

    //if a hit is outside the timing window we need to get rid of it
    while (m_first_hit_index < beatmap->m_hit_count && beatmap->m_hits[m_first_hit_index].time <= current_time - m_miss_hitwindow) {
        ++m_first_hit_index;
        player->ApplyRating(TAIKO_MISS);
    }

    while (m_first_effect_point < beatmap->m_effect_point_count && beatmap->m_effect_points[m_first_effect_point].time <= current_time - m_miss_hitwindow) {
        ++m_first_effect_point;
    }
}

void TaikoRuleset::Draw() {
    auto beatmap = GetBeatmap<TaikoBeatmap>();

    //current time in ms
    int current_time = (int)(GetPlayer()->GetTime() * 1000.f);

    DrawRectangle(0, 540/2 - 128/2, 960, 128, WHITE);
    DrawRectangle(0, 540/2 - 128/2 + 1, 960, 128-2, BLACK);
    DrawCircle(192, 540/2, 37, WHITE);
    DrawCircle(192, 540/2, 35, GRAY);

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

        DrawLine(192 + position*960, 540/2-64, 192 + position*960, 540/2+64, GRAY);
    }

    //Draw hits
    int out_count = 0;
    for (int i = m_first_hit_index; i < beatmap->m_hit_count; ++i) {
        TaikoHit *hit = &(beatmap->m_hits[i]);
        float position = TimeToPosition(hit->time, current_time);

        //We want to stop drawing hits that are outside the view at some point.
        //Idk how to do this correctly, so let's just stop whenever we have 15 consecutive hits outside.
        if (position >= 1) {
            //++out_count;
            //if (out_count >= 15) break;
            continue;
        }
        //out_count = 0;

        DrawCircle((int)(192 + position*960), 540/2, 35.f + 25.f*((hit->hit_type&TaikoHitType::Strong) != 0), hit->hit_type&TaikoHitType::Blue ? BLUE : RED);
    }
}


//////////////////////////////////////////
///PRIVATE

float TaikoRuleset::TimeToPosition(int time, int current_time) {
    float normalized_time = (float)(time-current_time) / 1900.f; //1900 found by comparing side to side with lazer

    TaikoEffectPoint *effect_point = GetEffectPointForTime(time);

    return normalized_time * GetBeatmap<TaikoBeatmap>()->m_base_velocity * effect_point->scroll_multiplier;
}

TaikoEffectPoint *TaikoRuleset::GetEffectPointForTime(int time) {
    TaikoEffectPoint *effect_points = GetBeatmap<TaikoBeatmap>()->m_effect_points;
    int effect_point_count = GetBeatmap<TaikoBeatmap>()->m_effect_point_count;

    //idk do these just in case
    if (time < effect_points[0].time) return &effect_points[0];
    if (time >= effect_points[effect_point_count-1].time) return &effect_points[effect_point_count-1];

    //FIXME: this is an attempt to optimize effect point lookups for objects located after the current timing point
    int first_i;
    if (m_first_effect_point >= effect_point_count || m_first_effect_point == 0 || time < effect_points[m_first_effect_point].time) first_i = 0;
    else first_i = m_first_effect_point-1; //Don't check past timing points when possible

    for (int i = first_i; i < effect_point_count-1; ++i) {
        if (time >= effect_points[i].time && time < effect_points[i+1].time) return &effect_points[i];
    }

    //This should never get reached, but doing it just in case
    return &effect_points[GetBeatmap<TaikoBeatmap>()->m_effect_point_count-1];
}

void TaikoRuleset::ComputeHitWindows() {
    auto beatmap = GetBeatmap<TaikoBeatmap>();
    float accuracy;
    if (beatmap == nullptr) accuracy = 5.f;
    else accuracy = beatmap->m_accuracy;

    //https://osu.ppy.sh/wiki/en/Beatmap/Overall_difficulty#osu!taiko
    m_great_hitwindow = 50 - 3 * accuracy;
    m_ok_hitwindow = (accuracy <= 5.f) ? (int)(120.f - 8.f * accuracy) : (int)(110.f - 6.f * accuracy);
    m_miss_hitwindow = (accuracy <= 5.f) ? (int)(135.f - 8.f * accuracy) : (int)(120.f - 5.f * accuracy);
}

//Return false if input was handled by this
bool TaikoRuleset::HandleHit(const RulesetInputMessage &message) {
    auto beatmap = GetBeatmap<TaikoBeatmap>();
    BeatmapPlayer *player = GetPlayer();

    //Check if there are some hits remaining
    if (m_first_hit_index >= beatmap->m_hit_count) return false;

    //Get the next hit
    TaikoHit *hit = &(beatmap->m_hits[m_first_hit_index]);

    //Get the time difference between the keypress and a perfect (0ms) hit
    int hit_relative_time = hit->time - message.time;

    //Check if we can reach it
    if (hit_relative_time < m_miss_hitwindow || hit_relative_time > m_miss_hitwindow) return false;

    if (message.action == Middle1 || message.action == Middle2) {
        //Wrong color (hit is blue, should be red)
        if ((hit->hit_type&TaikoHitType::Blue) != 0) {
            player->ApplyRating(TAIKO_MISS);
            return true;
        }

        player->SendHitDifference(hit_relative_time);

        if (hit_relative_time >= -m_great_hitwindow && hit_relative_time <= m_great_hitwindow) player->ApplyRating(TAIKO_GREAT);
        else if (hit_relative_time >= -m_ok_hitwindow && hit_relative_time <= m_ok_hitwindow) player->ApplyRating(TAIKO_OK);
        else player->ApplyRating(TAIKO_MISS);
        return true;
    }

    if (message.action == Side1 || message.action == Side2) {
        //Wrong color (hit is red, should be blue)
        if ((hit->hit_type&TaikoHitType::Blue) == 0) {
            player->ApplyRating(TAIKO_MISS);
            return true;
        }

        player->SendHitDifference(hit_relative_time);

        if (hit_relative_time >= -m_great_hitwindow && hit_relative_time <= m_great_hitwindow) player->ApplyRating(TAIKO_GREAT);
        else if (hit_relative_time >= -m_ok_hitwindow && hit_relative_time <= m_ok_hitwindow) player->ApplyRating(TAIKO_OK);
        else player->ApplyRating(TAIKO_MISS);
        return true;
    }

    return false;
}
