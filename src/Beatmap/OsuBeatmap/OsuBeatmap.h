#ifndef OSUBEATMAP_H
#define OSUBEATMAP_H

#include <cstddef>

#include "../Beatmap.h"

struct OsuTimingPoint;
struct OsuHitObject;

class OsuBeatmap : public Beatmap {
public:
    static OsuBeatmap *load(const char *path);
    ~OsuBeatmap();

    //[General]
    char m_audio_filename[32];
    int m_audio_lead_in;    //Milliseconds of silence before the audio starts playing
    int m_preview_time;     //Time in milliseconds when the audio preview should start
    //ignore Countdown
    char m_sample_set; //Sample set that will be used if timing points do not override it (Normal (0), Soft (1), Drum (2))
    //ignore StackLeniency
    int m_mode;             //Game mode (0 = osu!, 1 = taiko, 2 = catch, 3 = mania)
    //ignore LetterboxInBreaks
    //ignore WidescreenStoryboard

    //[Metadata]
    char m_title[32];
    char m_title_unicode[32];
    char m_artist[32];
    char m_artist_unicode[32];
    char m_creator[32];
    char m_version[32];  //diff name

    //[Difficulty]
    float m_hp_drain;
    float m_circle_size;
    float m_overall_difficulty; //Accuracy in lazer's editor
    float m_approach_rate;
    float m_slider_multiplier;
    float m_slider_tick_rate;

    //[Events]

    //[TimingPoints]
    OsuTimingPoint *m_timing_points;
    size_t m_timing_point_count;
    size_t m_timing_point_parsed;

    //[HitObjects]
    OsuHitObject *m_hit_objects;
    size_t m_hit_object_count;
    size_t m_hit_object_parsed;

    size_t CountObjectsOfType(int type);
    size_t CountTimingPoints(bool uninherited);

private:
    OsuBeatmap();
};

#endif //TAIKOBEATMAP_H
