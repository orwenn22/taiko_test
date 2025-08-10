#include "OsuTimingPoint.h"

OsuTimingPoint::OsuTimingPoint() : OsuTimingPoint(0, 1, 4, 0, 0, 100, false, 0) {}

OsuTimingPoint::OsuTimingPoint(int time, float beat_length, int meter, int sample_set, int sample_index, int volume, bool uninherited, int effects)
    : time(time), beat_length(beat_length), meter(meter), sample_set(sample_set), sample_index(sample_index), volume(volume), uninherited(uninherited), effects(effects) {}

OsuTimingPoint::~OsuTimingPoint() = default;

float OsuTimingPoint::GetScrollSpeedMultiplier() {
    if (uninherited) return 1.f;
    if (beat_length >= 0) return 1.f; //Maybe this is wrong?
    return BeatlengthToScrollMultiplier(beat_length);
}

float BeatlengthToScrollMultiplier(float beat_length) {
    return -100.f/beat_length;
}
