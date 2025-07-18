#ifndef TIME_H
#define TIME_H
#include <cstdint>

//TODO: these two are mostly used by the input thread in multithreading, maybe they shouldn't even be used/compiled at
//      all when building in singlethread mode.

//Should be called at the end of each frame
void SetLastFrameTimeToNow();

//get milliseconds since the end of last frame.
//when multithreading is enabled, this is called by the input thread to get the timestamp of each keypress.
uint64_t GetTimestampSinceLastFrameMs();

#endif //TIME_H
