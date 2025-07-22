#include "BeatmapPlayer.h"

#include <cstdlib>

#include "../Beatmap/Beatmap.h"
#include "../Ruleset/Ruleset.h"



BeatmapPlayer::BeatmapPlayer(Ruleset *ruleset, Beatmap *beatmap)
    : m_ruleset(ruleset), m_beatmap(beatmap),
      m_time(-2.f), m_judgements(nullptr), m_max_rating(0), m_current_rating(0), m_bonus_rating(0),
      m_combo(0), m_max_combo(0), m_health(100.f),
      m_music_started(false) {
    m_latest_hit_difference = 0;
    m_have_hit_difference = false;

    m_ruleset->m_player = this;
    m_ruleset->m_beatmap = beatmap;

    m_judgements = (int *) malloc(sizeof(int) * m_ruleset->m_rating_count);
    for (int i = 0; i < m_ruleset->m_rating_count; ++i) m_judgements[i] = 0;

    m_ruleset->LoadResources();
    m_ruleset->OnGameStart();
}

BeatmapPlayer::~BeatmapPlayer() {
    free(m_judgements);
    delete m_beatmap;
    delete m_ruleset;
}


void BeatmapPlayer::HandleInput(const InputEvent &input) {
    //Time of the previous frame
    int last_time_ms = (int)(m_time*1000.f);

    //TODO: maybe in the case of multithreading we should add dt to the input's timestamp?
    int press_time = last_time_ms+(int)input.timestamp_ms;
    //printf("press_time: %d ms\n", press_time);

    for (int i = 0; i < m_ruleset->m_input_count; ++i) {
        if (input.key == (int) m_ruleset->m_inputs[i].key) m_ruleset->HandleInput({m_ruleset->m_inputs[i].action, press_time});
    }
}


void BeatmapPlayer::Update(float dt) {
    if (m_ruleset == nullptr) return;

    m_ruleset->Update(dt);
    m_time += dt;

    const float audio_offset = 0.f; //TODO: this should be configurable in the future
    if (!m_music_started && m_time >= audio_offset) {
        m_ruleset->StartAudio(m_time-audio_offset);
        m_music_started = true;
    }
}


void BeatmapPlayer::ApplyRating(int rating) {
    if (rating < 0 || rating >= m_ruleset->m_rating_count) return;

    RulesetRating ruleset_rating = m_ruleset->m_ratings[rating];

    // Update combo
    if (ruleset_rating.combo_break) m_combo = 0;
    if (ruleset_rating.increase_combo) {
        ++m_combo;
        if (m_combo > m_max_combo) m_max_combo = m_combo;
    }

    // Update judgement
    m_judgements[rating] += 1;

    // Update rating
    if (ruleset_rating.bonus) {
        m_bonus_rating += ruleset_rating.rating;
    }
    else { //not bonus
        m_current_rating += ruleset_rating.rating;
        m_max_rating += ruleset_rating.max_rating;
    }
}

void BeatmapPlayer::SendHitDifference(int hit_difference) {
    m_latest_hit_difference = hit_difference;
    m_have_hit_difference = true;
}
