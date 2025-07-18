#ifndef RULESET_H
#define RULESET_H
#include <cstdint>


class BeatmapPlayer;
class Beatmap;

struct RulesetRating {
    //Name of rating/judgement
    const char *name;

    //The rating awarded by this judgement, used for acc calculation
    int rating;

    //The maximum rating if the note was hit perfectly
    //This is added to m_max_rating in BeatmapPlayer regardless of if the note was successfully hit or not.
    int max_rating;

    //Whether this rating should count as bonus or not
    //If set to true m_max_rating in BeatmapPlayer won't be inceased when the judgement is applied
    bool bonus;

    //Whether this judgement should increase the combo when it is applied.
    //If set to true then combo_break should be false
    bool increase_combo;

    //Whether this judgement should break combo when it is applied.
    //If this is set to true then increase_combo should be false
    bool combo_break;

    //The color used to display the rating in the UI
    uint32_t color;
};

//This is used to define the keybinds and their associated actions
struct RulesetInput {
    uint32_t key;
    uint32_t action;
};

//When one of the keys of the ruleset's m_actions is pressed this struct is sent to the HandleInput method of the ruleset
struct RulesetInputMessage {
    uint32_t action;
    int time; //time at which the action occurred relative to the start of the beatmap in ms
};

// Some default ratings as an example, these shouldn't be used directly.
extern RulesetRating default_ruleset_rating[];

class Ruleset {
public:
    Ruleset(RulesetRating *ratings = default_ruleset_rating, int rating_count = 2, RulesetInput *inputs = nullptr, int input_count = 0);
    virtual ~Ruleset();

    //Called when after the beatmap is loaded
    virtual void OnBeatmapLoaded();

    //Called when the game is starting
    virtual void OnGameStart();

    //The player will call this to know if the map is over. If it is, it should display some kind of result screen in the future.
    virtual bool IsOver();

    //Called when the game ended
    virtual void OnGameEnd();

    //The player will call this to know if the play is failed. If it is, it should display a game over screen in the future.
    virtual bool ShouldFail();

    //Handle input from the player
    virtual void HandleInput(const RulesetInputMessage &message) = 0;

    //Update logic
    virtual void Update(float dt) = 0;

    //Renderring logic
    virtual void Draw() = 0;

    template<typename BeatmapT>
    BeatmapT *GetBeatmap() { return (BeatmapT *) m_beatmap; }

    BeatmapPlayer *GetPlayer() { return m_player; }

private:
    friend BeatmapPlayer;

    Beatmap *m_beatmap;
    BeatmapPlayer *m_player;

    RulesetRating *m_ratings;
    int m_rating_count;

    RulesetInput *m_inputs;
    int m_input_count;
};



#endif //RULESET_H
