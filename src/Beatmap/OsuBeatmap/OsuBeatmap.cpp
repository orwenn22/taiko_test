#include "OsuBeatmap.h"

#include <cstdio>
#include <cstdlib>

#include "OsuHitObject.h"
#include "OsuTimingPoint.h"

OsuBeatmap::OsuBeatmap() : m_audio_filename{}, m_title{}, m_title_unicode{}, m_artist{}, m_artist_unicode{}, m_creator{}, m_version{} {
    m_audio_lead_in = 0;
    m_preview_time = -1;
    m_sample_set = 0;
    m_mode = 0;

    m_hp_drain = 5;
    m_circle_size = 5;
    m_overall_difficulty = 5;
    m_approach_rate = 5;
    m_slider_multiplier = 1;
    m_slider_tick_rate = 1;

    m_timing_points = nullptr;
    m_timing_point_count = 0;
    m_timing_point_parsed = 0;

    m_hit_objects = nullptr;
    m_hit_object_count = 0;
    m_hit_object_parsed = 0;
}

OsuBeatmap::~OsuBeatmap() {
    if (m_hit_objects) {
        for (size_t i = 0; i < m_hit_object_count; ++i) { m_hit_objects[i].~OsuHitObject(); }
        free(m_hit_objects);
        m_hit_objects = nullptr;
        m_hit_object_count = 0;
    }

    if (m_timing_points) {
        //Call destructor here?
        free(m_timing_points);
        m_timing_points = nullptr;
        m_timing_point_count = 0;
    }
}


size_t OsuBeatmap::CountObjectsOfType(int type) {
    size_t count = 0;
    for (size_t i = 0; i < m_hit_object_count; ++i) {
        count += m_hit_objects[i].IsOfType((OsuHitObjectType)type);
    }
    return count;
}

size_t OsuBeatmap::CountTimingPoints(bool uninherited) {
    size_t count = 0;
    for (size_t i = 0; i < m_timing_point_count; ++i) {
        count += m_timing_points[i].uninherited == uninherited;
    }
    return count;
}

