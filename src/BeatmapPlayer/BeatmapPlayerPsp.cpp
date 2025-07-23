#include "BeatmapPlayerPsp.h"

#include <cstdio>

#include "../Platform/Psp/Core.h"
#include "../Platform/Psp/Graphics/2d.h"

BeatmapPlayerPsp::BeatmapPlayerPsp(Ruleset *ruleset, Beatmap *beatmap) : BeatmapPlayer(ruleset, beatmap) {
}

BeatmapPlayerPsp::~BeatmapPlayerPsp() = default;

void BeatmapPlayerPsp::Draw() {
    if (m_ruleset == nullptr) return;

    m_ruleset->Draw();

    char buf[32];
    RulesetRating *ratings = m_ruleset->GetRatings();
    int rating_count = m_ruleset->GetRatingCount();

    int y = 2;
    for (int i = 0; i < rating_count; i++) {
        snprintf(buf, 32, "%s \x1\xff\xff\xff%d", ratings[i].name, m_judgements[i]);
        DrawText(buf, g_default_font, {2.f, (float)y}, 0.f, 1.f, ratings[i].color);
        y+=20;
    }
}
