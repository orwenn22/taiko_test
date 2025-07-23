#include "TaikoRuleset.h"

#include <cstdio>

#include "../../BeatmapPlayer/BeatmapPlayer.h"
#include "../../Beatmap/TaikoBeatmap/TaikoBeatmap.h"

#include "../../Beatmap/TaikoBeatmap/TaikoDrumRoll.h"
#include "../../Beatmap/TaikoBeatmap/TaikoEffectPoint.h"
#include "../../Beatmap/TaikoBeatmap/TaikoHit.h"
#include "../../Beatmap/TaikoBeatmap/TaikoTimingPoint.h"

static RulesetRating s_taiko_ratings[] = {
    {"Great", 300, 300, false, true, false, 0xFFFF5050},
    {"Ok", 150, 300, false, true, false, 0xFF50FF50},
    {"Miss", 0, 300, false, false, true, 0xFF0000FF},
    {"StrongBonus", 10, 10, true, false, false, 0xFFFFFFFF}
};
static const int s_taiko_rating_count = sizeof(s_taiko_ratings) / sizeof(RulesetRating);

#define TAIKO_GREAT 0
#define TAIKO_OK 1
#define TAIKO_MISS 2
#define TAIKO_STRONG_BONUS 3

TaikoRuleset::TaikoRuleset(RulesetInput *inputs, int input_count)
    : Ruleset(s_taiko_ratings, s_taiko_rating_count, inputs, input_count),
      m_first_hit_index(0), m_first_drum_roll(0), m_first_effect_point(0),
      m_latest_strong_hit_key(TaikoUnused), m_latest_strong_hit_time(-100000) {
    ComputeHitWindows();
}

TaikoRuleset::~TaikoRuleset() {

}

void TaikoRuleset::LoadResources() {
    Ruleset::LoadResources();
    ComputeHitWindows();
}

void TaikoRuleset::OnGameStart() {
    Ruleset::OnGameStart();
    GetPlayer()->m_health = 0.f; //Taiko health start at 0
}

bool TaikoRuleset::IsOver() {
    TaikoBeatmap *beatmap = GetBeatmap<TaikoBeatmap>();
    if (beatmap == nullptr) return true;
    return (m_first_hit_index >= beatmap->m_hit_count && m_first_drum_roll >= beatmap->m_drum_roll_count);
}

void TaikoRuleset::OnGameEnd() {
    Ruleset::OnGameEnd();
}

bool TaikoRuleset::ShouldFail() {
    if (IsOver() && GetPlayer()->m_health < 50.f) return true;
    return false;
}

void TaikoRuleset::HandleInput(const RulesetInputMessage &message) {
    if (HandleStrongNote(message)) {
        return;
    }
    if (HandleHit(message)) {
        ++m_first_hit_index;
        return;
    }
}

void TaikoRuleset::Update(float dt) {
    auto beatmap = GetBeatmap<TaikoBeatmap>();
    BeatmapPlayer *player = GetPlayer();

    //current time in ms
    int current_time = (int)(GetPlayer()->GetTime() * 1000.f);

    //if a hit is outside the timing window (too late) we need to get rid of it
    while (m_first_hit_index < beatmap->m_hit_count && beatmap->m_hits[m_first_hit_index].time <= current_time - m_miss_hitwindow) {
        ++m_first_hit_index;
        player->ApplyRating(TAIKO_MISS);
    }

    //We also want to get rid of drum rolls once they are fully over
    while (m_first_drum_roll < beatmap->m_drum_roll_count && beatmap->m_drum_rolls[m_first_drum_roll].GetEndTime() <= current_time) {
        ++m_first_drum_roll;
    }

    while (m_first_effect_point < beatmap->m_effect_point_count && beatmap->m_effect_points[m_first_effect_point].time <= current_time - m_miss_hitwindow) {
        ++m_first_effect_point;
    }
}


//////////////////////////////////////////
///PRIVATE

float TaikoRuleset::TimeToPosition(int time, int current_time) {
    return TimeToPosition(time, current_time, time);
}

float TaikoRuleset::TimeToPosition(int time, int current_time, int effect_time) {
    float normalized_position_no_effects = (float)(time-current_time) / 3000.f; //3000 found by comparing side to side with lazer

    //Get the corresponding effect point in order to apply the scroll effect
    TaikoEffectPoint *effect_point = GetBeatmap<TaikoBeatmap>()->GetEffectPointForTime(effect_time);

    //Ok, so i had no idea BPM affects scrolling, shoutout to this guy https://osu.ppy.sh/community/forums/topics/1851087?n=1
    TaikoTimingPoint *timing_point = GetBeatmap<TaikoBeatmap>()->GetTimingPointForTime(effect_time);

    float normalized_position = normalized_position_no_effects * GetBeatmap<TaikoBeatmap>()->m_base_velocity * effect_point->scroll_multiplier * (timing_point->GetBPM()/60);

    //Here we could apply additional scroll effects
    //if (normalized_position >= 1) return normalized_position; //don't apply effects on notes that are outside the screen
    //normalized_position = normalized_position*normalized_position;
    //normalized_position = powf(normalized_position*2-1, 3)/2 + 0.5;
    //normalized_position = -6 * powf(normalized_position-0.40825f, 2) + 1;

    return normalized_position;
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


bool TaikoRuleset::HandleStrongNote(const RulesetInputMessage &message) {
    if (m_latest_strong_hit_key == TaikoUnused) return false;

    int time_difference = message.time - m_latest_strong_hit_time;

    //We want to allow a bit of delay between the two presses for a strong hit, since pressing 2 keys EXACTLY at the same
    //time is too hard
    if (time_difference < -25 || time_difference > 25) return false;

   bool strong_hit_handled = false;

    //Check if the key that was pressed is the other one
    if ((message.action == Side1 && m_latest_strong_hit_key == Side2) ||
        (message.action == Side2 && m_latest_strong_hit_key == Side1) ||
        (message.action == Middle1 && m_latest_strong_hit_key == Middle2) ||
        (message.action == Middle2 && m_latest_strong_hit_key == Middle1)) {
        GetPlayer()->ApplyRating(TAIKO_STRONG_BONUS);
        strong_hit_handled = true;
    }

    m_latest_strong_hit_time = -100000;
    m_latest_strong_hit_key = TaikoUnused;

    return strong_hit_handled;
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
    if (hit_relative_time < -m_miss_hitwindow || hit_relative_time > m_miss_hitwindow) return false;

    if (message.action == Middle1 || message.action == Middle2) {
        //Wrong color (hit is blue, should be red)
        if (hit->IsBlue()) {
            player->ApplyRating(TAIKO_MISS);
            return true;
        }

        //TODO: maybe this shouldn't be included if we are hitting in the miss time window
        if (hit->IsStrong()) {
            m_latest_strong_hit_time = message.time;
            m_latest_strong_hit_key = (TaikoActions) message.action;
        }

        player->SendHitDifference(hit_relative_time);

        if (hit_relative_time >= -m_great_hitwindow && hit_relative_time <= m_great_hitwindow) player->ApplyRating(TAIKO_GREAT);
        else if (hit_relative_time >= -m_ok_hitwindow && hit_relative_time <= m_ok_hitwindow) player->ApplyRating(TAIKO_OK);
        else player->ApplyRating(TAIKO_MISS);
        return true;
    }

    if (message.action == Side1 || message.action == Side2) {
        //Wrong color (hit is red, should be blue)
        if (!hit->IsBlue()) {
            player->ApplyRating(TAIKO_MISS);
            return true;
        }

        //TODO: maybe this shouldn't be included if we are hitting in the miss time window
        if (hit->IsStrong()) {
            m_latest_strong_hit_time = message.time;
            m_latest_strong_hit_key = (TaikoActions) message.action;
        }

        player->SendHitDifference(hit_relative_time);

        if (hit_relative_time >= -m_great_hitwindow && hit_relative_time <= m_great_hitwindow) player->ApplyRating(TAIKO_GREAT);
        else if (hit_relative_time >= -m_ok_hitwindow && hit_relative_time <= m_ok_hitwindow) player->ApplyRating(TAIKO_OK);
        else player->ApplyRating(TAIKO_MISS);
        return true;
    }

    return false;
}
