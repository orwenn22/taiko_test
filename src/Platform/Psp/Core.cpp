#include "Core.h"

#include <pspaudio.h>
#include <pspaudiolib.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <psploadexec.h>
#include <psptypes.h>
#include <psprtc.h>
#include <pspthreadman.h>

#include "FrameTimesInfo.h"
#include "Audio/AudioThread.h"
#include "Graphics/2d.h"
#include "Graphics/Font.h"

//We want to update the FPS counter every .1 second (100k ticks)
#define FPS_UPDATE_INTERVAL 100000
#define FPS_MULTIPLIER (1000000.f/FPS_UPDATE_INTERVAL)

static FrameTimesInfo s_completed_frame_info, s_ongoing_frame_info;

static u64 s_last_tick = 0; //store when was the last time the fps counter was updated
static int s_frame_count = 0; //store the number of frames that occurred since s_last_tick
static u64 s_previous_frame_tick = 0; //store the tick of the start of the previous frame

static float s_fps = 0.f;
static float s_dt = 1.f/60.f;


char list[0x20000] __attribute__((aligned(64)));
Font *g_default_font = nullptr;
static int s_running = 1;


static int exit_callback(int arg1, int arg2, void *common) {
    s_running = 0;
    return 0;
}

static int callback_thread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

static int setup_callbacks(void) {
    int thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if(thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

static void InitGu(int draw_buffer_psm) {
    sceGuInit();

    //Set up buffers
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(draw_buffer_psm,(void*)0,BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH,SCREEN_HEIGHT,(void*)0x88000,BUFFER_WIDTH);
    sceGuDepthBuffer((void*)0x110000,BUFFER_WIDTH);

    //Set up viewport
    sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    //Set up depth buffer
    sceGuDepthRange(65535, 0);
    sceGuDepthFunc(GU_GEQUAL); //Depth buffer is reversed, so GEQUAL instead of LEQUAL
    sceGuClearDepth(0);
    //sceGuDepthRange(0, 65535);
    //sceGuDepthFunc(GU_LEQUAL);
    //sceGuClearDepth(0xFFFF);
    sceGuDepthMask(0); //Enable writting to depth buffer
    sceGuEnable(GU_DEPTH_TEST); //Enable depth testing

    sceGuAlphaFunc(GU_GREATER,0,0xff); //these two lines are needed to make transparent textures work
    sceGuEnable(GU_ALPHA_TEST);

    sceGuShadeModel(GU_SMOOTH); //Smooth shading

    sceGuFrontFace(GU_CW); //Front face is clockwise
    sceGuEnable(GU_CULL_FACE); //Cull faces

    //For alpha blending
    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

    sceGuFinish();
    sceGuDisplay(GU_TRUE);
}

static void EndGu() {
    sceGuDisplay(GU_FALSE);
    sceGuTerm();
}


void InitCore(int draw_buffer_psm) {
    s_running = 1;

    // Make exiting with the home button possible
    setup_callbacks();

    // Input-related stuff
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    // Setup audio
    pspAudioInit();
    pspAudioSetVolume(0, PSP_AUDIO_VOLUME_MAX/2, PSP_AUDIO_VOLUME_MAX/2);
    pspAudioSetChannelCallback(0, AudioCallback, nullptr);

    // Setup rendering
    InitGu(draw_buffer_psm);
    End2D();

    sceRtcGetCurrentTick(&s_previous_frame_tick);

    // Load default resources
    g_default_font = Font::FromPath("res/def_font.png", 8, 19);
}

int ShouldClose() {
    if (s_running) {
        s_completed_frame_info = s_ongoing_frame_info;

        UpdateTime();
        //UpdateInputState();
        sceRtcGetCurrentTick(&s_ongoing_frame_info.frame_start);
    }
    return s_running;
}

void EndCore() {
    delete g_default_font;
    g_default_font = nullptr;

    EndGu();
    pspAudioSetChannelCallback(0, NULL, NULL);
    pspAudioEnd();
}


void StartFrame() {
    sceGuStart(GU_DIRECT, list);
    sceRtcGetCurrentTick(&s_ongoing_frame_info.frame_start_draw);
}

void EndFrame() {
    sceRtcGetCurrentTick(&s_ongoing_frame_info.frame_end_draw);

    sceGuFinish();
    sceGuSync(0, 0);

    sceRtcGetCurrentTick(&s_ongoing_frame_info.frame_end_gpu_execute);

    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();

    sceRtcGetCurrentTick(&s_ongoing_frame_info.frame_end);
}

void UpdateTime() {
    //Get current tick
    u64 current_tick;
    sceRtcGetCurrentTick(&current_tick);

    //FPS computation
    u64 tick_diff = current_tick - s_last_tick;
    s_frame_count += 1;
    if (tick_diff >= FPS_UPDATE_INTERVAL) {
        s_fps = (float) s_frame_count * FPS_UPDATE_INTERVAL / (float) tick_diff * FPS_MULTIPLIER;
        s_frame_count = 0;
        s_last_tick = current_tick;
    }

    //DT computation
    tick_diff = current_tick - s_previous_frame_tick;
    s_dt = (float)tick_diff / 1000000.f;

    s_previous_frame_tick = current_tick;
}

float GetFPS() { return s_fps; }
float GetDeltaTime() { return s_dt; }

FrameTimesInfo GetPreviousFrameTimesInfo() {
    return s_completed_frame_info;
}
