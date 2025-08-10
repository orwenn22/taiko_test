#ifndef CORE_H
#define CORE_H

#define BUFFER_WIDTH 512
#define BUFFER_HEIGHT 272
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT BUFFER_HEIGHT

#include "FrameTimesInfo.h"

struct Font;
extern Font *g_default_font;
extern Font *g_pico_font;

void InitCore(int draw_buffer_psm);
int ShouldClose();
void EndCore();

void StartFrame();
void EndFrame();

//This is called every frame at the start of the main loop. It shouldn't be called anywhere else
void UpdateTime();
float GetFPS();
float GetDeltaTime();

FrameTimesInfo GetPreviousFrameTimesInfo();

#endif //CORE_H
