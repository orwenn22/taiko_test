#ifndef INPUTTHREAD_H
#define INPUTTHREAD_H

#include <cstdint>
#include <cstddef>
#include <queue>

#include "../../Raylib/Config.h"

struct InputEvent {
    int key;
    uint64_t timestamp_ms;  // Time in milliseconds since last frame
};


//Send a list of key for the input thread to scan (instead of scanning for everything each frames)
//Maybe this is not that usefull since scanning all the keyboard doesn't seem to impact performance, but keeping that just in case
void SendInputsToScan(uint64_t *keys, size_t count);

//Should be called at the start of the main loop, retrieve keyboard events and detect if the window's close button is pressed
void PollInputEvents(std::queue<InputEvent> &input_queue);

//Get the number of iterations the input thread performed since the last time that function was called.
//This is mostly meant for debugging and should probably go away in the future
int GetInputThreadIterations();

void InitInputHandling();
void CloseInputHandling();

#endif //INPUTTHREAD_H
