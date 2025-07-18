#include "TaikoBeatmap.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "TaikoEffectPoint.h"
#include "TaikoHit.h"
#include "../OsuBeatmap/OsuBeatmap.h"
#include "../OsuBeatmap/OsuHitObject.h"
#include "../OsuBeatmap/OsuTimingPoint.h"


TaikoBeatmap *TaikoBeatmap::FromOsuBeatmap(OsuBeatmap *osu_beatmap) {
    if (osu_beatmap == nullptr) return nullptr;
    if (osu_beatmap->m_mode != 1) return nullptr;
    if (osu_beatmap->m_timing_point_parsed != osu_beatmap->m_timing_point_count || osu_beatmap->m_timing_point_count == 0) return nullptr;
    if (osu_beatmap->m_hit_object_parsed != osu_beatmap->m_hit_object_count || osu_beatmap->m_hit_object_count == 0) return nullptr;

    TaikoBeatmap *taiko_beatmap = new TaikoBeatmap();

    //TODO : drum roll and the other weird thing

    //Allocate space for hits
    taiko_beatmap->m_hit_count = osu_beatmap->CountObjectsOfType(OsuHitObjectType::HitCircle);
    printf("TaikoBeatmap::FromOsuBeatmap() : %zu taiko hits\n", taiko_beatmap->m_hit_count);
    if (taiko_beatmap->m_hit_count != 0) {
        //Allocate stuff
        taiko_beatmap->m_hits = (TaikoHit *) malloc(sizeof(TaikoHit) * taiko_beatmap->m_hit_count);
    }

    //Convert all hit objects
    size_t converted_hits = 0;
    for (size_t i = 0; i < osu_beatmap->m_hit_object_count; ++i) {
        OsuHitObject *hit_object = &osu_beatmap->m_hit_objects[i];

        if (hit_object->IsOfType(OsuHitObjectType::HitCircle)) {
            if (converted_hits >= taiko_beatmap->m_hit_count) continue; //TODO: display error message
            //Convert to taiko hit
            taiko_beatmap->m_hits[converted_hits] = TaikoHit(hit_object->time, hit_object->hitsound);
            ++converted_hits;
        }
    }

    //Allocate space for effect points
    taiko_beatmap->m_effect_point_count = osu_beatmap->CountTimingPoints(false);
    printf("TaikoBeatmap::FromOsuBeatmap() : %zu taiko effect points\n", taiko_beatmap->m_effect_point_count);
    if (taiko_beatmap->m_effect_point_count != 0) {
        taiko_beatmap->m_effect_points = (TaikoEffectPoint *) malloc(sizeof(TaikoEffectPoint) * taiko_beatmap->m_effect_point_count);
    }

    //Convert all timing points
    size_t converted_effect_points = 0;
    for (size_t i = 0; i < osu_beatmap->m_timing_point_count; ++i) {
        OsuTimingPoint *timing_point = &osu_beatmap->m_timing_points[i];

        if (timing_point->uninherited) {
            //TODO : handle BPM timing point if necessary?
        }
        else {
            //Convert to taiko effect point
            taiko_beatmap->m_effect_points[converted_effect_points] = TaikoEffectPoint(timing_point->time, timing_point->GetScrollSpeedMultiplier());
            //printf("TaikoBeatmap::FromOsuBeatmap() : effect(%d,x%f)\n", timing_point->time, timing_point->GetScrollSpeedMultiplier());
            ++converted_effect_points;
        }
    }

    taiko_beatmap->m_base_velocity = osu_beatmap->m_slider_multiplier;
    taiko_beatmap->m_accuracy = osu_beatmap->m_overall_difficulty;
    strncpy(taiko_beatmap->m_audio_filename, osu_beatmap->m_audio_filename, 32);

    printf("TaikoBeatmap::FromOsuBeatmap() : converted hits: %zu/%zu\n", converted_hits, taiko_beatmap->m_hit_count);
    printf("TaikoBeatmap::FromOsuBeatmap() : converted effect points: %zu/%zu\n", converted_effect_points, taiko_beatmap->m_effect_point_count);

    return taiko_beatmap;
}

TaikoBeatmap::TaikoBeatmap(): m_audio_filename{} {
    m_hits = nullptr;
    m_hit_count = 0;

    m_effect_points = nullptr;
    m_effect_point_count = 0;

    m_base_velocity = 1.f;
    m_accuracy = 5.f;
}

TaikoBeatmap::~TaikoBeatmap() {
    if (m_hits) {
        free(m_hits);
        m_hits = nullptr;
        m_hit_count = 0;
    }

    if (m_effect_points) {
        free(m_effect_points);
        m_effect_points = nullptr;
        m_effect_point_count = 0;
    }
}
