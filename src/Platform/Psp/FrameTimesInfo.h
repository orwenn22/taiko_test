#ifndef TAIKO_TEST_FRAMETIMESINFO_H
#define TAIKO_TEST_FRAMETIMESINFO_H

#include <psptypes.h>

#include "Vector2.h"

//Mostly intended for performance monitoring
struct FrameTimesInfo {
    u64 frame_start; //contains the tick of the start of the fram
    u64 frame_start_draw; //contains the tick at which we finished updating stuff and start sendinc commands to the gpu
    u64 frame_end_draw; //contains the tick at which we finished sending commands to the gpu
    u64 frame_end_gpu_execute; //contains the tick at which the gpu finished renderring
    u64 frame_end; //contains the tick at which the draw and display buffer have been fully swapped (end of frame)
};

void DrawFrameTimesInfo(const FrameTimesInfo &frame_times_info ,Vector2<float> pos, float z, float width);

#endif //TAIKO_TEST_FRAMETIMESINFO_H