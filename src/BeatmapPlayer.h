#ifndef BEATMAPPLAYER_H
#define BEATMAPPLAYER_H
#include "InputThread.h"
#include "TaikoRuleset.h"
#include "TaikoBeatmap/TaikoBeatmap.h"


class Beatmap;
class Ruleset;

class BeatmapPlayer {
public:
    BeatmapPlayer(Ruleset *ruleset, Beatmap *beatmap);
    ~BeatmapPlayer();

    void HandleInput(const InputEvent &input);
    void Update(float dt);
    void Draw();

    float GetTime() { return m_time; }

    //Apply a judgement i.e. when a note or object was hit or missed
    void ApplyRating(int rating);

    //Send hit difference in ms.
    //In the future i might add the judgement bar, so it would be cool to use this to display judgement on that bar
    //For now it only displays the lateest hit diff on the screen
    void SendHitDifference(int hit_difference);

    float m_health;

private:
    Ruleset *m_ruleset;
    Beatmap *m_beatmap;
    float m_time;

    int *m_judgements;
    int m_max_rating;     //Exclude bonus
    int m_current_rating; //Exclude bonus
    int m_bonus_rating;

    int m_combo;
    int m_max_combo;

    //This is temporary
    int m_latest_hit_difference;
    bool m_have_hit_difference;
};



#endif //BEATMAPPLAYER_H
