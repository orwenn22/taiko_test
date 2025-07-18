#ifndef INPUTTHREAD_H
#define INPUTTHREAD_H

#include <cstdint>
#include <cstddef>
#include <queue>

#include "Config.h"

struct InputEvent {
    int key;
    uint64_t timestamp_ms;  // Time in milliseconds since game start
};

#if !SINGLE_THREAD_INPUT
void InputThreadMain();

//Send a list of key for the input thread to scan (instead of scanning for everything each frames)
void SendInputsToScan(uint64_t *keys, size_t count);
void ClearInputThread();

void PollFromInputThread(std::queue<InputEvent> &input_queue);
#endif

//Get the number of iterations the input thread performed since the last time that function was called
int GetInputThreadIterations();

#endif //INPUTTHREAD_H
