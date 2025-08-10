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
    InitCore(GU_PSM_8888);
    InitInputHandling();

    //NOTE: this assumes that the map file exists
    SceneManager *scene_manager = new SceneManager(BeatmapLoader::LoadFromFile<BeatmapPlayerPsp>("res/maps/circles/KIRA - Circles (feat. GUMI) (Amasugi) [Melancholy].osu"));
    //SceneManager *scene_manager = new SceneManager(BeatmapLoader::LoadFromFile<BeatmapPlayerPsp>("res/maps/null_spec/null_spec.osu"));

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
        snprintf(fps_buf, sizeof(fps_buf), "FPS %.0f", GetFPS()); //draw time
        DrawText(fps_buf, g_default_font, {SCREEN_WIDTH-80, 2.f}, 0.f, 1, 0xFF00AA00);
        DrawFrameTimesInfo(GetPreviousFrameTimesInfo(), {2.f, 2.f}, 0.f, 200.f);
        //SetLastFrameTimeToNow();
        EndFrame();
    }

    delete scene_manager;
    CloseInputHandling();
    EndCore();
    return 0;
}
