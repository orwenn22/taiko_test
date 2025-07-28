#ifndef BEATMAPPLAYER_H
#define BEATMAPPLAYER_H

#include "../Input/InputHandling.h"
#include "../Ruleset/Ruleset.h"
#include "../SceneManagement/Scene.h"


class Beatmap;
class Ruleset;

class BeatmapPlayer : public Scene {
public:
    BeatmapPlayer(Ruleset *ruleset, Beatmap *beatmap);
    ~BeatmapPlayer() override;

    void HandleInput(const InputEvent &input) override;
    void Update(float dt) override;

    float GetTime() { return m_time; }

    //Apply a judgement i.e. when a note or object was hit or missed
    void ApplyRating(int rating);

    //Send hit difference in ms.
    //In the future i might add the judgement bar, so it would be cool to use this to display judgement on that bar
    //For now it only displays the lateest hit diff on the screen
    void SendHitDifference(int hit_difference);

    float m_health;

protected:
    Ruleset *m_ruleset;
    Beatmap *m_beatmap;

    //Time since start of beatmap (in seconds)
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

    //TODO: maybe the ruleset should keep track of if the music is playing or not instead of the player?
    bool m_music_started;
};



#endif //BEATMAPPLAYER_H
