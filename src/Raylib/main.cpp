#include <atomic>
#include <chrono>
#include <cstring>
#include <thread>
#include <bits/shared_ptr_atomic.h>

#include "Config.h"
#include "../Base/Input/InputHandling.h"
#include "raylib.h"
#include "../Time.h"
#include "../Base/BeatmapPlayer/BeatmapLoader.h"
#include "BeatmapPlayer/BeatmapPlayerRaylib.h"
#include "../Base/MapIndex/MapIndexFile.h"
#include "../Base/SceneManagement/SceneManager.h"

std::atomic<bool> g_running = true;


void CorrectCWD(const char *argv0) {
    char buf[512];
    realpath(argv0, buf);

    size_t len = strlen(buf);
    size_t last_slash = len;
    for (size_t i = 0; i < len; ++i) {
        if (buf[i] == '/') last_slash = i;
    }
    buf[last_slash] = 0;

    printf("setting current dir: %s\n", buf);
    chdir(buf);
}

int main(int argc, const char *argv[]) {
    if (argv[0] == nullptr) return 1;
    CorrectCWD(argv[0]);

    //if (!MapIndexExist()) {
        GenerateMapIndexFile("./res/maps");
    //}


    SetLastFrameTimeToNow();

    //SetConfigFlags(FLAG_WINDOW_RESIZABLE); //crashes on multithreaded mode, let's not do that
    InitWindow(960, 540, "taco test");
    InitAudioDevice();
    SetTargetFPS(60);

    InitInputHandling();

    //NOTE: this assumes that the map file exists
    SceneManager *scene_manager = new SceneManager(BeatmapLoader::LoadFromFile<BeatmapPlayerRaylib>("res/maps/null_spec/null_spec.osu"));

    while (!WindowShouldClose() && g_running.load()) {
        //Get inputs
        std::queue<InputEvent> input_queue;
        PollInputEvents(input_queue);

        while (!input_queue.empty()) {
            scene_manager->HandleInput(input_queue.front());
            input_queue.pop();
        }

        scene_manager->Update(GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);
        scene_manager->Draw();
        DrawFPS(10, 10);

#if !SINGLE_THREAD_INPUT
        int input_iterations = GetInputThreadIterations();
        //if (input_iterations < 5) printf("Low input thread iteration count\n");
        //DrawText(TextFormat("%d IPF", input_iterations), 90, 10, 20, GREEN);
#endif

        SetLastFrameTimeToNow();
        EndDrawing(); //This calls it what makes this tread wait in order to reach the target framerate, therefore we should call SetLastFrameTimeToNow() right before (actually maybe it could be called at the end of PollInputEvents()? idk)
    }

    delete scene_manager;
    g_running = false;

    CloseInputHandling();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
