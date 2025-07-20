#ifndef TAIKORULESET_H
#define TAIKORULESET_H

#include "../Ruleset.h"

struct TaikoEffectPoint;

enum TaikoActions {
    TaikoUnused = 0,
    Middle1,
    Middle2,
    Side1,
    Side2
};

class TaikoRuleset : public Ruleset {
public:
    TaikoRuleset(RulesetInput *inputs, int input_count);
    ~TaikoRuleset() override;

    void LoadResources() override;
    void OnGameStart() override;
    bool IsOver() override;
    void OnGameEnd() override;
    bool ShouldFail() override;
    void HandleInput(const RulesetInputMessage &message) override;
    void Update(float dt) override;
    //void Draw() override;


protected:

    //1 = completely outside the playfield
    //0 = perfectly alligned with the target
    //time is the time from the start of the beatmap at which the hitobject is supposed to be hit (in ms)
    //current time correspond the the current position/progress of the map
    float TimeToPosition(int time, int current_time);

    //Same as above, but allow to specify a different time for which effect point should be used
    //(meant for drum roll tail that always moves at the same speed as the head)
    float TimeToPosition(int time, int current_time, int effect_time);

    TaikoEffectPoint *GetEffectPointForTime(int time);

    void ComputeHitWindows();

    bool HandleStrongNote(const RulesetInputMessage &message);
    bool HandleHit(const RulesetInputMessage &message);

    //This is the index of the first hit that should be drawn on screen.
    //Every hit with a strictly lower index already have been judged and should not be visible.
    int m_first_hit_index;

    //Same as above, but for drum roll
    int m_first_drum_roll;

    //Same as above, but for effect point barlines
    int m_first_effect_point;

    int m_great_hitwindow;
    int m_ok_hitwindow;
    int m_miss_hitwindow;

    //store the first key (action) that was used to press the last strong note
    TaikoActions m_latest_strong_hit_key;

    //store when the last strong hit was pressed
    int m_latest_strong_hit_time;
};



#endif //TAIKORULESET_H
