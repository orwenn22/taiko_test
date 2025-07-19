#include "TaikoBeatmap.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "TaikoDrumRoll.h"
#include "TaikoEffectPoint.h"
#include "TaikoHit.h"
#include "TaikoTimingPoint.h"
#include "../OsuBeatmap/OsuBeatmap.h"
#include "../OsuBeatmap/OsuHitObject.h"
#include "../OsuBeatmap/OsuTimingPoint.h"


TaikoBeatmap *TaikoBeatmap::FromOsuBeatmap(OsuBeatmap *osu_beatmap) {
    if (osu_beatmap == nullptr) return nullptr;
    if (osu_beatmap->m_mode != 1) return nullptr;
    if (osu_beatmap->m_timing_point_parsed != osu_beatmap->m_timing_point_count || osu_beatmap->m_timing_point_count == 0) return nullptr;
    if (osu_beatmap->m_hit_object_parsed != osu_beatmap->m_hit_object_count || osu_beatmap->m_hit_object_count == 0) return nullptr;

    TaikoBeatmap *taiko_beatmap = new TaikoBeatmap();

    taiko_beatmap->m_base_velocity = osu_beatmap->m_slider_multiplier;
    taiko_beatmap->m_accuracy = osu_beatmap->m_overall_difficulty;
    strncpy(taiko_beatmap->m_audio_filename, osu_beatmap->m_audio_filename, 32);

    //Allocate space for effect points
    taiko_beatmap->m_effect_point_count = osu_beatmap->CountTimingPoints(false);
    printf("TaikoBeatmap::FromOsuBeatmap() : %zu taiko effect points\n", taiko_beatmap->m_effect_point_count);
    if (taiko_beatmap->m_effect_point_count != 0) {
        taiko_beatmap->m_effect_points = (TaikoEffectPoint *) malloc(sizeof(TaikoEffectPoint) * taiko_beatmap->m_effect_point_count);
    }

    //Allocate space for timing points
    taiko_beatmap->m_timing_point_count = osu_beatmap->CountTimingPoints(true);
    printf("TaikoBeatmap::FromOsuBeatmap() : %zu taiko timing points\n", taiko_beatmap->m_timing_point_count);
    if (taiko_beatmap->m_timing_point_count != 0) {
        taiko_beatmap->m_timing_points = (TaikoTimingPoint *) malloc(sizeof(TaikoTimingPoint) * taiko_beatmap->m_effect_point_count);
    }

    //Convert all osu timing points
    size_t converted_effect_points = 0;
    size_t converted_timing_points = 0; //(bmp timing points)
    for (size_t i = 0; i < osu_beatmap->m_timing_point_count; ++i) {
        OsuTimingPoint *timing_point = &osu_beatmap->m_timing_points[i];

        if (timing_point->uninherited) {
            //Convert to taiko timing point
            taiko_beatmap->m_timing_points[converted_timing_points] = TaikoTimingPoint(timing_point->time, timing_point->beat_length);
            ++converted_timing_points;
        }
        else {
            //Convert to taiko effect point
            taiko_beatmap->m_effect_points[converted_effect_points] = TaikoEffectPoint(timing_point->time, timing_point->GetScrollSpeedMultiplier());
            //printf("TaikoBeatmap::FromOsuBeatmap() : effect(%d,x%f)\n", timing_point->time, timing_point->GetScrollSpeedMultiplier());
            ++converted_effect_points;
        }
    }

    //TODO : drum roll and the other weird thing

    //Allocate space for hits
    taiko_beatmap->m_hit_count = osu_beatmap->CountObjectsOfType(OsuHitObjectType::HitCircle);
    printf("TaikoBeatmap::FromOsuBeatmap() : %zu taiko hits\n", taiko_beatmap->m_hit_count);
    if (taiko_beatmap->m_hit_count != 0) {
        taiko_beatmap->m_hits = (TaikoHit *) malloc(sizeof(TaikoHit) * taiko_beatmap->m_hit_count);
    }

    //Allocate space for drum rolls
    taiko_beatmap->m_drum_roll_count = osu_beatmap->CountObjectsOfType(OsuHitObjectType::Slider);
    printf("TaikoBeatmap::FromOsuBeatmap() : %zu drum rolls hits\n", taiko_beatmap->m_drum_roll_count);
    if (taiko_beatmap->m_drum_roll_count != 0) {
        taiko_beatmap->m_drum_rolls = (TaikoDrumRoll *) malloc(sizeof(TaikoDrumRoll) * taiko_beatmap->m_drum_roll_count);
    }

    //Convert all hit objects
    size_t converted_hits = 0;
    size_t converted_drum_rolls = 0;
    for (size_t i = 0; i < osu_beatmap->m_hit_object_count; ++i) {
        OsuHitObject *hit_object = &osu_beatmap->m_hit_objects[i];

        if (hit_object->IsOfType(OsuHitObjectType::HitCircle)) {
            if (converted_hits >= taiko_beatmap->m_hit_count) continue; //TODO: display error message
            //Convert to taiko hit
            taiko_beatmap->m_hits[converted_hits] = TaikoHit(hit_object->time, hit_object->hitsound);
            ++converted_hits;
        }
        else if (hit_object->IsOfType(OsuHitObjectType::Slider)) {
            if (converted_drum_rolls >= taiko_beatmap->m_drum_roll_count) continue; //TODO: error message

            //Get timing point for bpm of drum roll
            TaikoTimingPoint *timing_point = taiko_beatmap->GetTimingPointForTime(hit_object->time);
            if (timing_point == nullptr) continue; //TODO: error message

            //Get effect point for slider velocity of drum roll
            TaikoEffectPoint *effect_point = taiko_beatmap->GetEffectPointForTime(hit_object->time);
            if (effect_point == nullptr) continue; //TODO: error message

            //Get params
            OsuSliderHitObjectParams *slider_params = (OsuSliderHitObjectParams *) hit_object->object_params;
            if (slider_params == nullptr) continue; //TODO: error message (this is kinda critical lmao)

            //Compute drum roll duration in ms
            int drum_roll_duration = (int) slider_params->getDuration(taiko_beatmap->m_base_velocity, effect_point->scroll_multiplier, timing_point->beat_length);

            //Convert to taiko drum roll
            taiko_beatmap->m_drum_rolls[converted_drum_rolls] = TaikoDrumRoll(hit_object->time, drum_roll_duration, timing_point->beat_length);

            printf("Duration of drum roll %zu (at %i ms): %i ms\n    slider length: %f\n    base velocity: %f\n    scroll mult: %f\n    beat length: %f\n", converted_drum_rolls, hit_object->time, drum_roll_duration, slider_params->length,taiko_beatmap->m_base_velocity, effect_point->scroll_multiplier, timing_point->beat_length);
            printf("    ticks: %i\n", taiko_beatmap->m_drum_rolls[converted_drum_rolls].tick_count);
            ++converted_drum_rolls;
        }
    }

    printf("TaikoBeatmap::FromOsuBeatmap() : converted hits: %zu/%zu\n", converted_hits, taiko_beatmap->m_hit_count);
    printf("TaikoBeatmap::FromOsuBeatmap() : converted drum rolls: %zu/%zu\n", converted_drum_rolls, taiko_beatmap->m_drum_roll_count);
    printf("TaikoBeatmap::FromOsuBeatmap() : converted effect points: %zu/%zu\n", converted_effect_points, taiko_beatmap->m_effect_point_count);
    printf("TaikoBeatmap::FromOsuBeatmap() : converted timing points: %zu/%zu\n", converted_timing_points, taiko_beatmap->m_timing_point_count);

    return taiko_beatmap;
}

TaikoBeatmap::TaikoBeatmap(): m_audio_filename{} {
    m_hits = nullptr;
    m_hit_count = 0;

    m_effect_points = nullptr;
    m_effect_point_count = 0;

    m_timing_points = nullptr;
    m_timing_point_count = 0;

    m_base_velocity = 1.f;
    m_accuracy = 5.f;
}

TaikoBeatmap::~TaikoBeatmap() {
    if (m_hits) {
        free(m_hits);
        m_hits = nullptr;
        m_hit_count = 0;
    }

    if (m_drum_rolls) {
        free(m_drum_rolls);
        m_drum_rolls = nullptr;
        m_drum_roll_count = 0;
    }

    if (m_effect_points) {
        free(m_effect_points);
        m_effect_points = nullptr;
        m_effect_point_count = 0;
    }

    if (m_timing_points) {
        free(m_timing_points);
        m_timing_points = nullptr;
        m_timing_point_count = 0;
    }
}


///////////////////////////
///PRIVATE

TaikoTimingPoint *TaikoBeatmap::GetTimingPointForTime(int time) {
    if (m_timing_point_count == 0) return nullptr;

    //idk do these just in case
    if (time < m_timing_points[0].time) return &m_timing_points[0];
    if (time >= m_timing_points[m_timing_point_count-1].time) return &m_timing_points[m_timing_point_count-1];


    for (int i = 0; i < m_timing_point_count-1; ++i) {
        if (time >= m_timing_points[i].time && time < m_timing_points[i+1].time) return &m_timing_points[i];
    }

    //This should never get reached, but doing it just in case
    return &m_timing_points[m_timing_point_count-1];
}

TaikoEffectPoint * TaikoBeatmap::GetEffectPointForTime(int time) {
    if (m_effect_point_count == 0) return nullptr;

    //idk do these just in case
    if (time < m_effect_points[0].time) return &m_effect_points[0];
    if (time >= m_effect_points[m_effect_point_count-1].time) return &m_effect_points[m_effect_point_count-1];


    for (int i = 0; i < m_effect_point_count-1; ++i) {
        if (time >= m_effect_points[i].time && time < m_effect_points[i+1].time) return &m_effect_points[i];
    }

    //This should never get reached, but doing it just in case
    return &m_effect_points[m_effect_point_count-1];
}
