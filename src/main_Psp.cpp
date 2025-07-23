#include <cstdio>
#include <pspgu.h>
#include <pspkernel.h>
#include <chrono>

#include "Platform/Psp/Core.h"
#include "Platform/Psp/Graphics/2d.h"

#include "Beatmap/OsuBeatmap/OsuBeatmap.h"
#include "Beatmap/TaikoBeatmap/TaikoBeatmap.h"
#include "BeatmapPlayer/BeatmapPlayerPsp.h"
#include "Input/InputHandling.h"
#include "Ruleset/TaikoRuleset/TaikoRulesetPsp.h"

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

    BeatmapPlayerPsp *beatmap_player = new BeatmapPlayerPsp(new TaikoRulesetPsp, taiko_beatmap);

    std::queue<InputEvent> input_queue;
    while (ShouldClose()) {
        PollInputEvents(input_queue);
        while (!input_queue.empty()) {
            beatmap_player->HandleInput(input_queue.front());
            input_queue.pop();
        }

        beatmap_player->Update(GetDeltaTime());

        StartFrame();

        clearBackground(0xFF000000);

        beatmap_player->Draw();

        char fps_buf[32];
        snprintf(fps_buf, 32, "FPS: %.0f", GetFPS());
        DrawText(fps_buf, g_default_font, {SCREEN_WIDTH-80, 2.f}, 0.f, 1, 0xFF00AA00);

        //SetLastFrameTimeToNow();
        EndFrame();
    }

    //delete beatmap_player;

    CloseInputHandling();
    EndCore();
    return 0;
}
