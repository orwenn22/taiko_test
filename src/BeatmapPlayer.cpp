#include "BeatmapPlayer.h"

#include <cstdlib>

#include "main.h"
#include "raylib.h"
#include "Ruleset.h"
#include "Utils.h"



BeatmapPlayer::BeatmapPlayer(Ruleset *ruleset, Beatmap *beatmap) : m_ruleset(ruleset), m_beatmap(beatmap), m_time(.0f), m_judgements(nullptr), m_max_rating(0), m_current_rating(0), m_bonus_rating(0), m_combo(0), m_max_combo(0), m_health(100.f) {
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
}


void BeatmapPlayer::HandleInput(const InputEvent &input) {
    //we keep track of the time since the start of the beatmap in a float (
    int last_time_ms = (int)(m_time*1000.f);

    //TODO: maybe in the case of multithreading we should add dt to the input's timestamp?
    int press_time = last_time_ms+(int)input.timestamp_ms;
    //printf("press_time: %d ms\n", press_time);

    for (int i = 0; i < m_ruleset->m_input_count; ++i) {
        if (input.key == (int) m_ruleset->m_inputs[i].key) m_ruleset->HandleInput({m_ruleset->m_inputs[i].action, press_time});
    }
}


void BeatmapPlayer::Update(float dt) {
    m_ruleset->Update(dt);
    m_time += dt;
}

void BeatmapPlayer::Draw() {
    m_ruleset->Draw();

    int y = 28;
    for (int i = 0; i < m_ruleset->m_rating_count; ++i) {
        DrawText(m_ruleset->m_ratings[i].name, 10, y, 20, PspColorToRaylib(m_ruleset->m_ratings[i].color));
        DrawText(TextFormat("%d", m_judgements[i]), 145, y, 20, WHITE);
        y += 20;
    }

    DrawText("Acc", 10, y, 20, PINK);
    DrawText(TextFormat("%.2f%%", (float)m_current_rating / (float)m_max_rating * 100.f), 145, y, 20, WHITE);
    y += 20;

    DrawText("Combo", 10, y, 20, YELLOW);
    DrawText(TextFormat("%d", m_combo), 145, y, 20, WHITE);
    y += 20;

    DrawText("Time", 10, y, 20, WHITE);
    DrawText(TextFormat("%.2f (Over: %d)", m_time, m_ruleset->IsOver()), 145, y, 20, WHITE);
    y += 20;

    if (m_have_hit_difference) {
        DrawText(TextFormat("%d ms", m_latest_hit_difference), 10, 340, 20, YELLOW);
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
