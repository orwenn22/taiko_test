#ifndef TAIKOTIMINGPOINT_H
#define TAIKOTIMINGPOINT_H


struct TaikoTimingPoint {
    int time;
    float beat_length; //length of a beat in ms

    TaikoTimingPoint(int time, float beat_length);
    ~TaikoTimingPoint();

    //TODO: since this is used by the scroll algorithm, maybe it would be worth it to cache it for all timing points at the creation of the object?
    float GetBPM();
};


#endif //TAIKOTIMINGPOINT_H
