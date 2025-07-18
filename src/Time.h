#ifndef TIME_H
#define TIME_H
#include <cstdint>

void SetGameStartTimeToNow();
void SetLastFrameTimeToNow();

//get milliseconds since game start
uint64_t GetTimestampMs();
uint64_t GetTimestampSinceLastFrameMs();

#endif //TIME_H
