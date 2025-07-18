#ifndef TAIKOHIT_H
#define TAIKOHIT_H


enum TaikoHitType {
    Blue = 0b00000001,
    Strong = 0b00000010,
};

//Object type 1
struct TaikoHit {
    int time;
    unsigned char hit_type;

    TaikoHit(int time, bool blue, bool strong);
    TaikoHit(int time, unsigned char osu_hitsounds);
    ~TaikoHit();
};



#endif //TAIKOHIT_H
