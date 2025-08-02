#include <cstdio>
#include <pspgu.h>
#include <pspkernel.h>
#include <cstring>

#include "Platform/Psp/Core.h"
#include "Platform/Psp/Graphics/2d.h"

#include "Beatmap/OsuBeatmap/OsuBeatmap.h"
#include "Beatmap/TaikoBeatmap/TaikoBeatmap.h"
#include "BeatmapPlayer/BeatmapPlayerPsp.h"
#include "Input/InputHandling.h"
#include "Platform/Psp/Audio/AudioStreamMP3.h"
#include "Registry/FileFormat/FileFormatRegistry.h"
#include "Ruleset/TaikoRuleset/TaikoRulesetPsp.h"
#include "SceneManagement/SceneManager.h"

PSP_MODULE_INFO("taco_test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);


//TODO: move this somewhere else
void clearBackground(unsigned int color) {
    sceGuClearColor(color);
    //the depth buffer clear should already be setup correctly before calling this
    sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
}


int main(int argc, const char *argv[]) {
    InitCore();
    InitInputHandling();

    //this is to demonstrate how to load a beatmap using the file format registry thing,
    //might need some cleanup in the future, but at least this is format/ruleset independent
    Scene *scene = nullptr;
    Beatmap *beatmap = nullptr;
    const FileFormatEntry *file_format = MatchFileFormatFromFilename("res/maps/null_spec/null_spec.osu", &beatmap);
    if (file_format != nullptr && beatmap != nullptr) {
        Ruleset *ruleset = file_format->make_ruleset();
        scene = new BeatmapPlayerPsp(ruleset, beatmap);
    }
    SceneManager *scene_manager = new SceneManager(scene);

    std::queue<InputEvent> input_queue;
    while (ShouldClose()) {
        PollInputEvents(input_queue);
        while (!input_queue.empty()) {
            scene_manager->HandleInput(input_queue.front());
            input_queue.pop();
        }

        scene_manager->Update(GetDeltaTime());

        StartFrame();

        clearBackground(0xFF000000);

        scene_manager->Draw();

        char fps_buf[32];
        snprintf(fps_buf, 32, "FPS: %.0f", GetFPS());
        DrawText(fps_buf, g_default_font, {SCREEN_WIDTH-80, 2.f}, 0.f, 1, 0xFF00AA00);

        //SetLastFrameTimeToNow();
        EndFrame();
    }

    delete scene_manager;
    CloseInputHandling();
    EndCore();
    return 0;
}
