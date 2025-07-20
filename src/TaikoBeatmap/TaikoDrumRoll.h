#ifndef TAIKODRUMROLL_H
#define TAIKODRUMROLL_H
#include <cmath>


struct TaikoDrumRoll {
    int time; //in ms
    int duration; //in ms
    int beat_length; //in ms
    bool strong;

    int tick_count; //amount of ticks in the drum roll (doesn't include the tick on the head of the drum droll)


    TaikoDrumRoll(int time, int duration, int beat_length, bool strong);
    ~TaikoDrumRoll();

    //Get the end time of the drum roll relative to the start of the beatmap
    inline int GetEndTime() { return time + duration; }
};



#endif //TAIKODRUMROLL_H
