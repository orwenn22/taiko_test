#include <cstdio>
#include <pspgu.h>
#include <pspkernel.h>
#include <cstring>
#include <psprtc.h>

#include "Platform/Psp/Core.h"
#include "Platform/Psp/Graphics/2d.h"

#include "BeatmapPlayer/BeatmapLoader.h"
#include "BeatmapPlayer/BeatmapPlayerPsp.h"
#include "Input/InputHandling.h"
#include "Platform/Psp/Audio/AudioStreamMP3.h"
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

    //NOTE: this assumes that the map file exists
    SceneManager *scene_manager = new SceneManager(BeatmapLoader::LoadFromFile<BeatmapPlayerPsp>("res/maps/circles/KIRA - Circles (feat. GUMI) (Amasugi) [Melancholy].osu"));

    u64 tick_postdraw_previous = 0;
    u64 tick_preinput = 0;
    u64 tick_preupdate = 0;
    u64 tick_predraw = 0;
    u64 tick_postdraw = 0;
    std::queue<InputEvent> input_queue;
    while (ShouldClose()) {
        tick_postdraw_previous = tick_postdraw;

        sceRtcGetCurrentTick(&tick_preinput);

        PollInputEvents(input_queue);
        while (!input_queue.empty()) {
            scene_manager->HandleInput(input_queue.front());
            input_queue.pop();
        }

        sceRtcGetCurrentTick(&tick_preupdate);
        scene_manager->Update(GetDeltaTime());

        sceRtcGetCurrentTick(&tick_predraw);
        StartFrame();

        clearBackground(0xFF000000);

        scene_manager->Draw();
        sceRtcGetCurrentTick(&tick_postdraw);

        char fps_buf[70];
        snprintf(fps_buf, sizeof(fps_buf), "FPS %.0f\nW %llu\nI %llu\nU %llu\nD %llu",
            GetFPS(),
            tick_preinput-tick_postdraw_previous, //wait time
            tick_preupdate-tick_preinput, //input time
            tick_predraw-tick_preupdate, //update time
            tick_postdraw-tick_predraw); //draw time
        DrawText(fps_buf, g_default_font, {SCREEN_WIDTH-80, 2.f}, 0.f, 1, 0xFF00AA00);
        //SetLastFrameTimeToNow();
        EndFrame();
    }

    delete scene_manager;
    CloseInputHandling();
    EndCore();
    return 0;
}
