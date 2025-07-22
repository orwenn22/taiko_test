#ifndef BEATMAPPLAYERRAYLIB_H
#define BEATMAPPLAYERRAYLIB_H
#include "BeatmapPlayer.h"


class BeatmapPlayerRaylib : public BeatmapPlayer {
public:
    BeatmapPlayerRaylib(Ruleset *ruleset, Beatmap *beatmap);
    ~BeatmapPlayerRaylib() override;

    void Draw() override;
};



#endif //BEATMAPPLAYERRAYLIB_H
