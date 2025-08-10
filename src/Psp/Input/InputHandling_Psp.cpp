#include "../ConfigPsp.h"
#if PSP_INPUT_SINGLE_THREAD

#include <pspctrl.h>

#include "../../Base/Input/InputHandling.h"

static SceCtrlData s_pad = {0}; //input state of current frame
static SceCtrlData s_pad_old = {0}; //input state of previous frame;

void SendInputsToScan(uint64_t *keys, size_t count) {
    //Since there are not many keys on the psp let's do nothing for now
}

void PollInputEvents(std::queue<InputEvent> &input_queue) {
    s_pad_old = s_pad; //save previous frame state
    sceCtrlReadBufferPositive(&s_pad, 1); //FIXME (?): this is a blocking operation, maybe it would be better to move that to a different thread
    for (uint32_t key = PSP_CTRL_SELECT; key < PSP_CTRL_HOME; key <<= 1) {
        if ((key&s_pad.Buttons) != 0 && (key&s_pad_old.Buttons) == 0) input_queue.emplace(key, 0);
    }

}

void SetLastPollTimeToNow() { /* nothing */ }

//Get the number of iterations the input thread performed since the last time that function was called.
//This is mostly meant for debugging and should probably go away in the future
int GetInputThreadIterations() {
    return 0;
}

void InitInputHandling() { /* nothing */ }
void CloseInputHandling() { /* nothing */ }

#endif
