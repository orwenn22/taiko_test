#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <bits/shared_ptr_atomic.h>

#include "BeatmapPlayer.h"
#include "Config.h"
#include "InputThread.h"
#include "raylib.h"
#include "TaikoRuleset.h"
#include "Time.h"
#include "OsuBeatmap/OsuBeatmap.h"
#include "TaikoBeatmap/TaikoBeatmap.h"

std::atomic<bool> g_running = true;


int main() {
    OsuBeatmap *beatmap = OsuBeatmap::load("res/enthousiastic.txt");
    TaikoBeatmap *taiko_beatmap = TaikoBeatmap::FromOsuBeatmap(beatmap);
    delete beatmap;
    if (taiko_beatmap == nullptr) return 1;

    SetGameStartTimeToNow();

    InitWindow(960, 540, "taco test");
    InitAudioDevice();
    SetTargetFPS(60);

#if !SINGLE_THREAD_INPUT
    std::thread input_thread(InputThreadMain);
#endif

    BeatmapPlayer *player = new BeatmapPlayer(new TaikoRuleset, taiko_beatmap);

    while (!WindowShouldClose() && g_running.load()) {
#if !SINGLE_THREAD_INPUT
        std::queue<InputEvent> input_queue;
        PollFromInputThread(input_queue);
        while (!input_queue.empty()) {
            player->HandleInput(input_queue.front());
            input_queue.pop();
        }
#else
        PollInputEvents();
#endif

        player->Update(GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);
        player->Draw();
        DrawFPS(10, 10);
        DrawText(TextFormat("%d IPF", GetInputThreadIterations()), 90, 10, 20, GREEN);
        EndDrawing();
    }

    g_running = false;

#if !SINGLE_THREAD_INPUT
    input_thread.join();
    ClearInputThread();
#endif

    CloseAudioDevice();
    CloseWindow();

    delete taiko_beatmap;
    return 0;
}
