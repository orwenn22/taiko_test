#include <cstdio>
#include <pspgu.h>
#include <pspkernel.h>
#include <cstring>
#include <psprtc.h>

#include "Platform/Core.h"
#include "Platform/Graphics/2d.h"

#include "../Base/BeatmapPlayer/BeatmapLoader.h"
#include "BeatmapPlayer/BeatmapPlayerPsp.h"
#include "../Base/Input/InputHandling.h"
#include "Platform/Audio/AudioStreamMP3.h"
#include "../Base/SceneManagement/SceneManager.h"

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
        snprintf(fps_buf, sizeof(fps_buf), "IPF %i", GetInputThreadIterations());
        DrawText(fps_buf, g_default_font, {SCREEN_WIDTH-80, 22.f}, 0.f, 1, 0xFF00AA00);
        DrawFrameTimesInfo(GetPreviousFrameTimesInfo(), {2.f, 2.f}, 0.f, 200.f);

        EndFrame();
    }

    delete scene_manager;
    CloseInputHandling();
    EndCore();
    return 0;
}
