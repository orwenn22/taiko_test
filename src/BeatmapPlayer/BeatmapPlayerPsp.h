#ifndef BEATMAPPLAYERPSP_H
#define BEATMAPPLAYERPSP_H
#include "BeatmapPlayer.h"


class BeatmapPlayerPsp : public BeatmapPlayer {
public:
    BeatmapPlayerPsp(Ruleset *ruleset, Beatmap *beatmap);
    ~BeatmapPlayerPsp() override;

    void Draw() override;
};



#endif //BEATMAPPLAYERPSP_H
