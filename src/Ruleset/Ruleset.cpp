#include "Ruleset.h"

#include <raylib.h>

RulesetRating default_ruleset_rating[] = {
    {"Perfect", 300, false, false, true, false, 0xFFFF5050},
    {"Miss", 0, false, false, false, true, 0xFF0000FF}
};


Ruleset::Ruleset(RulesetRating *ratings, int rating_count, RulesetInput *inputs, int input_count) : m_beatmap(nullptr), m_player(nullptr), m_ratings(ratings), m_rating_count(rating_count), m_inputs(inputs), m_input_count(input_count) {
}

Ruleset::~Ruleset() = default;

void Ruleset::LoadResources() { /* nothing */ }
void Ruleset::OnGameStart() { /* nothing */ }

//TODO: maybe a default behavior to start the audio should be implemented in Ruleset?
void Ruleset::StartAudio(float offset) { /* nothing */ }

bool Ruleset::IsOver() { return true; }
void Ruleset::OnGameEnd() { /* noting */ }
bool Ruleset::ShouldFail() { return false; }
