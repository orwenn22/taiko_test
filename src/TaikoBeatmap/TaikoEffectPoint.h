#ifndef TAIKOEFFECTPOINT_H
#define TAIKOEFFECTPOINT_H



struct TaikoEffectPoint {
    int time;
    float scroll_multiplier;

    TaikoEffectPoint(int time, float scroll_multiplier);
    ~TaikoEffectPoint();
};



#endif //TAIKOEFFECTPOINT_H
