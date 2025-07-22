#ifndef CORE_H
#define CORE_H

#define BUFFER_WIDTH 512
#define BUFFER_HEIGHT 272
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT BUFFER_HEIGHT

struct Font;

extern Font *g_default_font;

void InitCore();
int ShouldClose();
void EndCore();

void StartFrame();
void EndFrame();

//This is called every frame at the start of the main loop. It shouldn't be called anywhere else
void UpdateTime();
float GetFPS();
float GetDeltaTime();

#endif //CORE_H
