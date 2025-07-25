#include "BeatmapPlayerPsp.h"

#include <cstdio>

#include "../Beatmap/Beatmap.h"
#include "../Platform/Psp/Audio/AudioThread.h"
#include "../Platform/Psp/Core.h"
#include "../Platform/Psp/Graphics/2d.h"

BeatmapPlayerPsp::BeatmapPlayerPsp(Ruleset *ruleset, Beatmap *beatmap) : BeatmapPlayer(ruleset, beatmap) {
}

BeatmapPlayerPsp::~BeatmapPlayerPsp() = default;

void BeatmapPlayerPsp::Draw() {
    if (m_ruleset == nullptr) return;

    m_ruleset->Draw();

    char buf[256];
    RulesetRating *ratings = m_ruleset->GetRatings();
    int rating_count = m_ruleset->GetRatingCount();

    //draw judgements at the top
    int y = 2;
    for (int i = 0; i < rating_count; i++) {
        snprintf(buf, 256, "%s \x1\xff\xff\xff%d", ratings[i].name, m_judgements[i]);
        DrawText(buf, g_default_font, {2.f, (float)y}, 0.f, 1.f, ratings[i].color);
        y+=20;
    }

    //draw these at the bottom
    y = SCREEN_HEIGHT-20-2;

    if (m_max_rating != 0) { //Avoid division by 0 (crashes on real hardware)
        snprintf(buf, 256, "Acc \x1\xff\xff\xff%.2f", ((float)m_current_rating/(float)m_max_rating)*100.f);
        DrawText(buf, g_default_font, {2.f, (float)y}, 0.f, 1.f, 0xFFC26DFF);
    }
    y -= 20;

    snprintf(buf, 256, "Combo \x1\xff\xff\xff%d", m_combo);
    DrawText(buf, g_default_font, {2.f, (float)y}, 0.f, 1.f, 0xFFFFFFFF);
    y -= 20;

    snprintf(buf, 256, "root: %s", m_beatmap->GetRootPath());
    DrawText(buf, g_default_font, {2.f, (float)y}, 0.f, 1.f, 0xFFFFFFFF);

    AudioStream *audio = GetCurrentAudioStream();
    if (audio) {
        int sample_rate = audio->GetSampleRate();
        sniprintf(buf, 256, "sample rate: %dhz\n", sample_rate);
        DrawText(buf, g_default_font, {SCREEN_WIDTH-160, 22.f}, 0.f, 1, (sample_rate == 44100) ? 0xFF00AA00 : 0xFF0000AA);
    }
}
