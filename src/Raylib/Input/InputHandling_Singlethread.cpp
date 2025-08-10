#include "../Config.h"
#if SINGLE_THREAD_INPUT

#include <cstdlib>

#include "../../Base/Input/InputHandling.h"
#include "../main.h"
#include "raylib.h"

static int *s_keys = nullptr; //Store all the keys we should check
static size_t s_count = 0; //Store the amount of keys in the array above
static std::queue<InputEvent> s_input_queue;

static void CheckAllKeys() {
    for (int i = 0; i < 128; ++i) {
        if (IsKeyPressed(i)) {
            s_input_queue.emplace(i, 0); //Maybe the timestamp should be set to dt?
        }
    }
}

static void CheckSpecifiedKeys() {
    for (size_t i = 0; i < s_count; ++i) {
        if (IsKeyPressed(s_keys[i])) {
            s_input_queue.push(InputEvent(s_keys[i], 0)); ////Maybe the timestamp should be set to dt?
        }
    }
}


void SendInputsToScan(int *keys, size_t count) {
    if (s_keys != nullptr) free(s_keys);
    s_keys = (int *)malloc(count * sizeof(int));
    for (size_t i = 0; i < count; i++) s_keys[i] = keys[i];
    s_count = count;
}

void PollInputEvents(std::queue<InputEvent> &input_queue) {
    PollInputEvents();

    if (WindowShouldClose()) g_running.store(false); //TODO: use a different variable for windows close request?


    if (s_keys == nullptr || s_count == 0) CheckAllKeys();
    else CheckSpecifiedKeys();

    while (!s_input_queue.empty()) {
        input_queue.push(s_input_queue.front());
        s_input_queue.pop();
    }
}

void SetLastPollTimeToNow() { /* nothing */ }

int GetInputThreadIterations() {
    return 0;
}


void InitInputHandling() {
    //nothing
}

void CloseInputHandling() {
    if (s_keys != nullptr) free(s_keys);
    s_keys = nullptr;
    s_count = 0;
}

#endif
