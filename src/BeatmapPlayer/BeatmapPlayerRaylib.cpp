#include "BeatmapPlayerRaylib.h"

#include <cstdio>

#include "../Platform/Raylib/UtilsRaylib.h"

#include <raylib.h>

#include "../Beatmap/Beatmap.h"


BeatmapPlayerRaylib::BeatmapPlayerRaylib(Ruleset *ruleset, Beatmap *beatmap) : BeatmapPlayer(ruleset, beatmap), m_background{} {
}

BeatmapPlayerRaylib::~BeatmapPlayerRaylib() {
    UnloadTexture(m_background);
}

bool BeatmapPlayerRaylib::LoadResourcesInternal() {
    if (!BeatmapPlayer::LoadResourcesInternal()) return false;

    if (m_beatmap != nullptr && m_beatmap->GetRootPath() && m_beatmap->GetBackground() != nullptr) {
        m_background = LoadTexture(TextFormat("%s/%s", m_beatmap->GetRootPath(), m_beatmap->GetBackground()));
    }

    return true;
}


void BeatmapPlayerRaylib::Draw() {
    if (m_ruleset == nullptr) return;

    if (m_background.id > 0) {
        DrawTexturePro(m_background, {0, 0, (float)m_background.width, (float)m_background.height},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            {0, 0}, 0,
            {70, 70, 70, 255});
    }

    m_ruleset->Draw();

    RulesetRating *rating = m_ruleset->GetRatings();
    int rating_count = m_ruleset->GetRatingCount();

    int y = 28;
    for (int i = 0; i < rating_count; ++i) {
        DrawText(rating[i].name, 10, y, 20, ABGR8888ColorToRaylib(rating[i].color));
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

    DrawText("BG", 10, y, 20, WHITE);
    DrawText(TextFormat("%s", m_beatmap->GetBackground()), 145, y, 20, WHITE);

    if (m_have_hit_difference) {
        DrawText(TextFormat("%d ms", m_latest_hit_difference), 10, 340, 20, YELLOW);
    }
}
