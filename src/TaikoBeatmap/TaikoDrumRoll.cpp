#include "TaikoDrumRoll.h"

TaikoDrumRoll::TaikoDrumRoll(int time, int duration, int beat_length) : time(time), duration(duration), beat_length(beat_length) {
    tick_count = (int)std::round((float)duration / ((float)beat_length/4.f));
}

TaikoDrumRoll::~TaikoDrumRoll() = default;
