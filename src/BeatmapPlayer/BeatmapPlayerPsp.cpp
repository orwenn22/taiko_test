#include "BeatmapPlayerPsp.h"

#include <cstdio>
#include <cstdlib>
#include <pspgum.h>
#include <psprtc.h>

#include "../Beatmap/Beatmap.h"
#include "../Platform/Psp/Audio/AudioThread.h"
#include "../Platform/Psp/Core.h"
#include "../Platform/Psp/Graphics/Texture.h"
#include "../Platform/Psp/Graphics/2d.h"
#include "../Platform/Psp/Graphics/Vertex.h"

//comment this to convert the background to ABGR4444 to save space in vram
//(also better performance, looks ugly though)
#define BG_ABGR8888


BeatmapPlayerPsp::BeatmapPlayerPsp(Ruleset *ruleset, Beatmap *beatmap) : BeatmapPlayer(ruleset, beatmap), m_background(nullptr) {
}

BeatmapPlayerPsp::~BeatmapPlayerPsp() {
    delete m_background;
    m_background = nullptr;
}

bool BeatmapPlayerPsp::LoadResourcesInternal() {
    if (!BeatmapPlayer::LoadResourcesInternal()) return false;

    LoadBackground();
    return true;
}

void BeatmapPlayerPsp::Draw() {
    if (m_ruleset == nullptr) return;

    DrawBackground();
    m_ruleset->Draw();

    char buf[256];
    RulesetRating *ratings = m_ruleset->GetRatings();
    int rating_count = m_ruleset->GetRatingCount();

    //draw judgements at the top
    int y = 6;
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
        DrawText(buf, g_default_font, {SCREEN_WIDTH-300, 22.f}, 0.f, 1, (sample_rate == 44100) ? 0xFF00AA00 : 0xFF0000AA);
    }
}


/////////////////////////////
///PRIVATE

static constexpr int background_width = SCREEN_WIDTH;
static constexpr int background_height = SCREEN_HEIGHT;

//TODO: move most of this logic in a function, and make it possible to specify any path
void BeatmapPlayerPsp::LoadBackground() {
    if ( m_beatmap == nullptr) return;
    if (m_beatmap->GetBackground() == nullptr) return; //no background

    if (m_background != nullptr) {
        delete m_background;
        m_background = nullptr;
    }

    char background_path[1024];
    snprintf(background_path, sizeof(background_path), "%s/%s", m_beatmap->GetRootPath(), m_beatmap->GetBackground());

    //load background from image
    Texture *background = Texture::Load(background_path);
    if (background == nullptr) {
        printf("Failed to load %s\n", background_path);
        return;
    }
    printf("First background loaded with a size of %d*%d\n", background->w, background->h);

    //resize to match display size
    Texture *resized_background = background->CopyAndResize(background_width, background_height, true, false);
    delete background;
    if (resized_background == nullptr) return;
    printf("Second background loaded with a size of %d*%d\n", resized_background->w, resized_background->h);
    //resized_background->Swizzle();

#ifndef BG_ABGR8888
    //convert to abgr4444
    Texture *converted_background = resized_background->CopyAndConvert(GU_PSM_4444);
    delete resized_background;
    if (converted_background == nullptr) return;
    printf("Converted background to ABGR4444\n");

    //copy abgr4444 background to vram
    converted_background->Swizzle();
    m_background = converted_background->CopyToVram((void *)0x04154000);
    delete converted_background;
#else
    //copy abgr8888 background to vram
    resized_background->Swizzle();
    m_background = resized_background->CopyToVram((void *)0x04154000);
    delete resized_background;
#endif

    if (m_background == nullptr) return;
    m_background->minimizing_filter = GU_LINEAR;
    m_background->magnifying_filter = GU_LINEAR;
}


void BeatmapPlayerPsp::DrawBackground() {
    if (m_background == nullptr) return;

    uint32_t color = 0xFF333333;

    static Vertex_UV32_Color32_XYZ32 vertices[] = {
        {0, 0, color, 0, 0, 0}, //top left
        {background_width, 0, color, SCREEN_WIDTH, 0, 0}, //top right
        {0, background_height, color, 0, SCREEN_HEIGHT, 0}, //bottom left
        {background_width, background_height, color, SCREEN_WIDTH, SCREEN_HEIGHT, 0}, //bottom right
    };

    static uint8_t indices[] = {
        0, 1, 2,
        1, 3, 2
    };

    GpuUseTexture(m_background);
    sceGuEnable(GU_TEXTURE_2D);
    sceGuDisable(GU_BLEND);
    sceGuDisable(GU_DEPTH_TEST);
    sceGumDrawArray(GU_TRIANGLES, Vertex_UV32_Color32_XYZ32::flags | GU_INDEX_8BIT | GU_TRANSFORM_3D, 6, indices, vertices);
    sceGuDisable(GU_TEXTURE_2D);
    sceGuEnable(GU_BLEND);
    sceGuEnable(GU_DEPTH_TEST);
}
