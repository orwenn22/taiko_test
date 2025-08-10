#include "../ConfigPsp.h"
#if !PSP_INPUT_SINGLE_THREAD

#include "../../Base/Input/InputHandling.h"

#include <pspctrl.h>
#include <pspkernel.h>
#include <psprtc.h>

#include <cstdio>

static SceUID s_thread_id = -1;
static volatile bool s_input_thread_running = false;

static SceCtrlData s_pad = {0}; //current input state
static SceCtrlData s_pad_old = {0}; //previous input state
static u64 s_last_poll_time = 0;
static std::queue<InputEvent> s_input_queue;
static SceLwMutexWorkarea s_input_mutex;

static volatile int s_iteration_count;


static int InputThreadMain(SceSize args, void *argp) {
    while (s_input_thread_running) {
        sceKernelLockLwMutex(&s_input_mutex, 1, nullptr);

        //get current time
        u64 current_time = 0;
        sceRtcGetCurrentTick(&current_time);

        //save previous input state
        s_pad_old = s_pad;
        sceCtrlReadBufferPositive(&s_pad, 1);

        //scan every key
        for (uint32_t key = PSP_CTRL_SELECT; key < PSP_CTRL_HOME; key <<= 1) {
            if ((key&s_pad.Buttons) != 0 && (key&s_pad_old.Buttons) == 0) s_input_queue.emplace(key, (current_time-s_last_poll_time)/1000);
        }

        s_iteration_count = s_iteration_count+1;
        sceKernelUnlockLwMutex(&s_input_mutex, 1);
        sceKernelDelayThread(2000);
    }

    sceKernelExitThread(0);
    return 0;
}

void PollInputEvents(std::queue<InputEvent> &input_queue) {
    sceKernelLockLwMutex(&s_input_mutex, 1, nullptr);
    while (!s_input_queue.empty()) {
        input_queue.emplace(s_input_queue.front());
        s_input_queue.pop();
    }

    sceRtcGetCurrentTick(&s_last_poll_time);

    sceKernelUnlockLwMutex(&s_input_mutex, 1);
}

void SetLastPollTimeToNow() {
    sceKernelLockLwMutex(&s_input_mutex, 1, nullptr);
    sceRtcGetCurrentTick(&s_last_poll_time);
    sceKernelUnlockLwMutex(&s_input_mutex, 1);
}

int GetInputThreadIterations() {
    int result = s_iteration_count;
    s_iteration_count = 0;
    return result;
}

void InitInputHandling() {
    if (sceKernelCreateLwMutex(&s_input_mutex, "input mutex", 0, 0, nullptr) != 0) {
        printf("Failed to create input mutex\n");
        return;
    }

    sceRtcGetCurrentTick(&s_last_poll_time);
    s_thread_id = sceKernelCreateThread("input_thread", InputThreadMain, 0x18, 0x8000, 0, nullptr);
    if (s_thread_id >= 0) {
        s_input_thread_running = true;
        sceKernelStartThread(s_thread_id, 0, nullptr);
    }
}

void CloseInputHandling() {
    if (!s_input_thread_running) return;

    s_input_thread_running = false;
    if (sceKernelWaitThreadEnd(s_thread_id, nullptr) < 0) {
        printf("Failed to wait for input thread to end\n");
    }

    if (sceKernelDeleteThread(s_thread_id) < 0) {
        printf("Failed to delete input thread\n");
    }

    sceKernelDeleteLwMutex(&s_input_mutex);
    s_thread_id = -1;
}

#endif