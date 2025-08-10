#ifndef TAIKOBEATMAP_H
#define TAIKOBEATMAP_H

#include <cstddef>

#include "../Beatmap.h"

struct TaikoDrumRoll;
struct TaikoTimingPoint;
struct TaikoEffectPoint;
struct TaikoHit;
class OsuBeatmap;

class TaikoBeatmap : public Beatmap {
public:
    static TaikoBeatmap *FromOsuBeatmap(OsuBeatmap *osu_beatmap);
    static TaikoBeatmap *FromOsuBeatmapPath(const char *file_path);
    ~TaikoBeatmap() override;

    TaikoTimingPoint *GetTimingPointForTime(int time);
    TaikoEffectPoint *GetEffectPointForTime(int time);

    TaikoHit *m_hits;
    size_t m_hit_count;

    TaikoDrumRoll *m_drum_rolls;
    size_t m_drum_roll_count;

    TaikoEffectPoint *m_effect_points;
    size_t m_effect_point_count;

    TaikoTimingPoint *m_timing_points;
    size_t m_timing_point_count;

    float m_base_velocity;
    float m_accuracy; //Allow to adjust hit windows
    char m_audio_filename[32];

private:
    TaikoBeatmap();
};

#endif //TAIKOBEATMAP_H
