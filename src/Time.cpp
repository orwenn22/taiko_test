#include "Time.h"

#include <chrono>
#include <mutex>

#include "Config.h"

static std::chrono::steady_clock::time_point s_game_start_time;
static std::chrono::steady_clock::time_point s_last_frame_time;

#if !SINGLE_THREAD_INPUT
static std::mutex s_game_start_time_mutex;
static std::mutex s_last_frame_time_mutex;
#endif


void SetGameStartTimeToNow() {
#if !SINGLE_THREAD_INPUT
    std::lock_guard<std::mutex> guard(s_game_start_time_mutex);
#endif
    s_game_start_time = std::chrono::steady_clock::now();
}

void SetLastFrameTimeToNow() {
#if !SINGLE_THREAD_INPUT
    std::lock_guard<std::mutex> guard(s_last_frame_time_mutex);
#endif
    s_last_frame_time = std::chrono::steady_clock::now();
}


uint64_t GetTimestampMs() {
#if !SINGLE_THREAD_INPUT
    std::lock_guard<std::mutex> guard(s_game_start_time_mutex);
#endif
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - s_game_start_time
    ).count();
}

uint64_t GetTimestampSinceLastFrameMs() {
#if !SINGLE_THREAD_INPUT
    std::lock_guard<std::mutex> guard(s_last_frame_time_mutex);
#endif
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - s_last_frame_time
    ).count();
}
