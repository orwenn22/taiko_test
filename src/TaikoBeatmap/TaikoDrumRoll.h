#ifndef TAIKODRUMROLL_H
#define TAIKODRUMROLL_H
#include <cmath>


struct TaikoDrumRoll {
    int time; //in ms
    int duration; //in ms
    int beat_length; //in ms

    int tick_count; //amount of ticks in the drum roll (doesn't include the tick on the head of the drum droll)

    //TODO: a drum roll can be strong

    TaikoDrumRoll(int time, int duration, int beat_length);
    ~TaikoDrumRoll();
};



#endif //TAIKODRUMROLL_H
