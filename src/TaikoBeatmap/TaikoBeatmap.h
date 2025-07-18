#ifndef TAIKOBEATMAP_H
#define TAIKOBEATMAP_H

#include <cstddef>

#include "../Beatmap.h"

struct TaikoEffectPoint;
struct TaikoHit;
class OsuBeatmap;

class TaikoBeatmap : public Beatmap {
public:
    static TaikoBeatmap *FromOsuBeatmap(OsuBeatmap *osu_beatmap);
    ~TaikoBeatmap();

    TaikoHit *m_hits;
    size_t m_hit_count;

    TaikoEffectPoint *m_effect_points;
    size_t m_effect_point_count;

    float m_base_velocity;
    float m_accuracy; //Allow to adjust hit windows
    char m_audio_filename[32];

private:
    TaikoBeatmap();
};

#endif //TAIKOBEATMAP_H
