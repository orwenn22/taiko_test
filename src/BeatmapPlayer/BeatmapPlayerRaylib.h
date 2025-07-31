#ifndef BEATMAPPLAYERRAYLIB_H
#define BEATMAPPLAYERRAYLIB_H
#include "BeatmapPlayer.h"

#include <raylib.h>

class BeatmapPlayerRaylib : public BeatmapPlayer {
public:
    BeatmapPlayerRaylib(Ruleset *ruleset, Beatmap *beatmap);
    ~BeatmapPlayerRaylib() override;

    void Draw() override;


private:
    Texture m_background;
};



#endif //BEATMAPPLAYERRAYLIB_H
