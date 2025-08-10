#ifndef BEATMAPPLAYERRAYLIB_H
#define BEATMAPPLAYERRAYLIB_H
#include "../../Base/BeatmapPlayer/BeatmapPlayer.h"

#include <raylib.h>

class BeatmapPlayerRaylib : public BeatmapPlayer {
public:
    BeatmapPlayerRaylib(Ruleset *ruleset, Beatmap *beatmap);
    ~BeatmapPlayerRaylib() override;

    bool LoadResourcesInternal() override;
    void Draw() override;


private:
    Texture m_background;
};



#endif //BEATMAPPLAYERRAYLIB_H
