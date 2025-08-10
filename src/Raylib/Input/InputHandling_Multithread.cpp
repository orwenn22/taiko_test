#include "../Config.h"
#if !SINGLE_THREAD_INPUT

#include "../../Base/Input/InputHandling.h"

#include <mutex>
#include <queue>
#include <bits/this_thread_sleep.h>
#include <raylib.h>
#include <thread>

#include "../main.h"
#include "../../Time.h"

static std::thread s_input_thread;

static int *s_keys = nullptr; //Store all the keys we should check
static size_t s_count = 0; //Store the amount of keys in the array above
static std::queue<InputEvent> s_input_queue; //This is how the input thread send stuff to the main thread
static std::mutex s_keys_mutex; //Mutex for all the things above

static std::atomic<int> s_input_thread_iterations(0);

static void CheckAllKeys() {
    for (int i = 0; i < 128; ++i) {
        if (IsKeyPressed(i)) {
            s_input_queue.emplace(i, GetTimestampSinceLastFrameMs());
        }
    }
}

static void CheckSpecifiedKeys() {
    for (size_t i = 0; i < s_count; ++i) {
        if (IsKeyPressed(s_keys[i])) {
            s_input_queue.push(InputEvent(s_keys[i], GetTimestampSinceLastFrameMs()));
        }
    }
}

static void InputThreadMain() {
    while (g_running.load()) {
        s_input_thread_iterations.fetch_add(1, std::memory_order_relaxed);
        PollInputEvents();

        if (WindowShouldClose()) g_running.store(false);

        std::lock_guard<std::mutex> lock(s_keys_mutex);

        if (s_keys == nullptr || s_count == 0) CheckAllKeys();
        else CheckSpecifiedKeys();

        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::this_thread::sleep_for(std::chrono::nanoseconds(100000)); //0.1ms
    }
}


void SendInputsToScan(int *keys, size_t count) {
    std::lock_guard<std::mutex> lock(s_keys_mutex);
    if (s_keys != nullptr) free(s_keys);
    s_keys = (int *)malloc(count * sizeof(int));
    for (size_t i = 0; i < count; i++) s_keys[i] = keys[i];
    s_count = count;
}


void PollInputEvents(std::queue<InputEvent> &input_queue) {
    std::lock_guard<std::mutex> lock(s_keys_mutex);
    while (!s_input_queue.empty()) {
        input_queue.push(s_input_queue.front());
        s_input_queue.pop();
    }
}


int GetInputThreadIterations() {
    int iterations = s_input_thread_iterations.load();
    s_input_thread_iterations.store(0);
    return iterations;
}


void InitInputHandling() {
    s_input_thread = std::thread(InputThreadMain);
}

void CloseInputHandling() {
    s_input_thread.join();

    std::lock_guard<std::mutex> lock(s_keys_mutex);
    if (s_keys != nullptr) free(s_keys);
    s_keys = nullptr;
    s_count = 0;
}

#endif
