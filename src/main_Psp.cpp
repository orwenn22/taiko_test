#include <cstdio>
#include <pspgu.h>
#include <pspkernel.h>
#include <chrono>

#include "Platform/Psp/Core.h"
#include "Platform/Psp/Graphics/2d.h"

#include "Beatmap/OsuBeatmap/OsuBeatmap.h"
#include "Beatmap/TaikoBeatmap/TaikoBeatmap.h"
#include "Input/InputHandling.h"

PSP_MODULE_INFO("taco_test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);

//TODO: move this somewhere else
void clearBackground(unsigned int color) {
    sceGuClearColor(color);
    //the depth buffer clear should already be setup correctly before calling this
    sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
}

int main() {
    InitCore();
    InitInputHandling();

    OsuBeatmap *beatmap = OsuBeatmap::load("res/lagtrain.txt");
    TaikoBeatmap *taiko_beatmap = TaikoBeatmap::FromOsuBeatmap(beatmap);
    delete beatmap;

    std::queue<InputEvent> input_queue;
    int input_queue_count = 0;
    int total_input_queue_count = 0;
    while (ShouldClose()) {
        input_queue_count = 0;
        PollInputEvents(input_queue);
        while (!input_queue.empty()) {
            ++input_queue_count;
            ++total_input_queue_count;
            input_queue.pop();
        }
        //update stuff here

        StartFrame();

        clearBackground(0xFF000000);
        DrawText("hi, this is a placeholder text\nfor the psp platform, hopefully\nthis becomes playable someday", g_default_font, {2.f, 2.f}, 0.f, 1.f, 0xFFFFFFFF);

        char buf[48];

        snprintf(buf, 48, "hits: %d", taiko_beatmap->m_hit_count);
        DrawText(buf, g_default_font, {2.f, 80.f}, 0.f, 1.f, 0xFFFFFFFF);

        sniprintf(buf, 48, "effect points: %d", taiko_beatmap->m_effect_point_count);
        DrawText(buf, g_default_font, {2.f, 100.f}, 0.f, 1.f, 0xFFFFFFFF);

        sniprintf(buf, 48, "timing points: %d", taiko_beatmap->m_timing_point_count);
        DrawText(buf, g_default_font, {2.f, 120.f}, 0.f, 1.f, 0xFFFFFFFF);

        snprintf(buf, 48, "drum rolls: %d", taiko_beatmap->m_drum_roll_count);
        DrawText(buf, g_default_font, {2.f, 140.f}, 0.f, 1.f, 0xFFFFFFFF);

        sniprintf(buf, 48, "input events: %d (%d)", input_queue_count, total_input_queue_count);
        DrawText(buf, g_default_font, {2.f, 160.f}, 0.f, 1.f, 0xFFFFFFFF);

        //SetLastFrameTimeToNow();
        EndFrame();
    }

    //TODO: remove this whenever we actually get the beatmap player on the psp
    delete taiko_beatmap;

    CloseInputHandling();
    EndCore();
    return 0;
}
