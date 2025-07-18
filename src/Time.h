#ifndef TIME_H
#define TIME_H
#include <cstdint>

void SetGameStartTimeToNow();

//get milliseconds since game start
uint64_t GetTimestampMs();

#endif //TIME_H
