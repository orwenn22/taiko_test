#include "TaikoTimingPoint.h"

TaikoTimingPoint::TaikoTimingPoint(int time, float beat_length) : time(time), beat_length(beat_length) {
}

TaikoTimingPoint::~TaikoTimingPoint() {
}

float TaikoTimingPoint::GetBPM() {
    return 60000.f / beat_length;
}
