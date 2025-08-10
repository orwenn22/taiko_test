#ifndef BEATMAPPLAYER_H
#define BEATMAPPLAYER_H

#include "../Input/InputHandling.h"
#include "../SceneManagement/Scene.h"
#include "../Ruleset/Ruleset.h"


class Beatmap;
class Ruleset;

class BeatmapPlayer : public Scene {
public:
    BeatmapPlayer(Ruleset *ruleset, Beatmap *beatmap);
    ~BeatmapPlayer() override;

    //This will load the resources of the beatmap player AND the ruleset, returns false if a critical resource failed to load
    //(aka, this will call LoadResourcesInternal() and m_ruleset->LoadResources())
    //should return false if a critical resource failed to load
    bool LoadResources();

    //Called by BeatmapPlayer::LoadResources(), should be called before OnGameStart()
    //This does nothing by default, but subclasses should overwrite this to load stuff from the beatmap (such as the
    //background image)
    //should return false if a critical resource failed to load
    virtual bool LoadResourcesInternal();

    //Just calls ruleset->OnGameStart();
    //It should be called after LoadResources() and before the first time Update()/Draw()
    void OnGameStart();

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
    bool m_resources_loaded;

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
