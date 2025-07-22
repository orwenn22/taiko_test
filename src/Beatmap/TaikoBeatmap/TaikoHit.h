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

    inline bool IsStrong() { return (hit_type & Strong) != 0; }
    inline bool IsBlue() { return (hit_type & Blue) != 0; }
};



#endif //TAIKOHIT_H
