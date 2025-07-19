#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <bits/shared_ptr_atomic.h>

#include "BeatmapPlayer.h"
#include "Config.h"
#include "InputHandling.h"
#include "raylib.h"
#include "TaikoRuleset.h"
#include "Time.h"
#include "OsuBeatmap/OsuBeatmap.h"
#include "TaikoBeatmap/TaikoBeatmap.h"

std::atomic<bool> g_running = true;


int main() {
    OsuBeatmap *beatmap = OsuBeatmap::load("res/sara_hito.txt");
    TaikoBeatmap *taiko_beatmap = TaikoBeatmap::FromOsuBeatmap(beatmap);
    delete beatmap;
    if (taiko_beatmap == nullptr) return 1;

    SetLastFrameTimeToNow();

    InitWindow(960, 540, "taco test");
    InitAudioDevice();
    SetTargetFPS(60);

    InitInputHandling();

    BeatmapPlayer *player = new BeatmapPlayer(new TaikoRuleset, taiko_beatmap);

    while (!WindowShouldClose() && g_running.load()) {
        //Get inputs
        std::queue<InputEvent> input_queue;
        PollInputEvents(input_queue);

        while (!input_queue.empty()) {
            player->HandleInput(input_queue.front());
            input_queue.pop();
        }

        player->Update(GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);
        player->Draw();
        DrawFPS(10, 10);

#if !SINGLE_THREAD_INPUT
        int input_iterations = GetInputThreadIterations();
        if (input_iterations < 5) printf("Low input thread iteration count\n");
        DrawText(TextFormat("%d IPF", input_iterations), 90, 10, 20, GREEN);
#endif

        SetLastFrameTimeToNow();
        EndDrawing(); //This calls it what makes this tread wait in order to reach the target framerate, therefore we should call SetLastFrameTimeToNow() right before (actually maybe it could be called at the end of PollInputEvents()? idk)
    }

    g_running = false;

    CloseInputHandling();
    CloseAudioDevice();
    CloseWindow();

    delete taiko_beatmap;
    return 0;
}
