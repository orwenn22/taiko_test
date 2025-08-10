#include "TaikoHit.h"

#include "../OsuBeatmap/OsuHitObject.h"

TaikoHit::TaikoHit(int time, bool blue, bool strong) : time(time), hit_type(blue | (strong << 1)) {}

TaikoHit::TaikoHit(int time, unsigned char osu_hitsounds) : time(time) {
    bool blue = (osu_hitsounds & (OsuHitSounds::Whistle | OsuHitSounds::Clap)) != 0;
    bool strong = (osu_hitsounds & OsuHitSounds::Finish) != 0;
    hit_type = blue | (strong << 1);
}

TaikoHit::~TaikoHit() = default;
