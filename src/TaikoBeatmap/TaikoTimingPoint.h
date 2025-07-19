#ifndef TAIKOTIMINGPOINT_H
#define TAIKOTIMINGPOINT_H


struct TaikoTimingPoint {
    int time;
    float beat_length; //length of a beat in ms

    TaikoTimingPoint(int time, float beat_length);
    ~TaikoTimingPoint();

    float GetBPM();
};


#endif //TAIKOTIMINGPOINT_H
