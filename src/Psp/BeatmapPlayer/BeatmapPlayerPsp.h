#ifndef BEATMAPPLAYERPSP_H
#define BEATMAPPLAYERPSP_H

#include "../../Base/BeatmapPlayer/BeatmapPlayer.h"


struct Texture;

class BeatmapPlayerPsp : public BeatmapPlayer {
public:
    BeatmapPlayerPsp(Ruleset *ruleset, Beatmap *beatmap);
    ~BeatmapPlayerPsp() override;

    bool LoadResourcesInternal() override;
    void Draw() override;


private:
    void LoadBackground();

    void DrawBackground();

    Texture *m_background;
};



#endif //BEATMAPPLAYERPSP_H
