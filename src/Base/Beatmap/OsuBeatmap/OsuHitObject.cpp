#include "OsuHitObject.h"

#include <cstdio>
#include <cstdlib>


OsuHitObjectParams::~OsuHitObjectParams() = default;

OsuSliderHitObjectParams::OsuSliderHitObjectParams() : curve_type('U'), curve_points(nullptr), curve_point_count(0), slides(1), length(1), edge_sounds(nullptr), edge_sound_count(0) {
}

OsuSliderHitObjectParams::~OsuSliderHitObjectParams() {
    if (curve_points) {
        free(curve_points);
        curve_points = nullptr;
        curve_point_count = 0;
    }
    if (edge_sounds) {
        free(edge_sounds);
        edge_sounds = nullptr;
        edge_sound_count = 0;
    }
}

OsuSpinnerHitObjectParams::OsuSpinnerHitObjectParams(int end_time) : end_time(end_time) {
}

OsuSpinnerHitObjectParams::~OsuSpinnerHitObjectParams() = default;

OsuHitObject::OsuHitObject(int x, int y, int time, int type, int hitsound) : x(x), y(y), time(time), type(type), object_params(nullptr), hitsound(hitsound) {}

OsuHitObject::~OsuHitObject() {
    if (object_params) {
        delete object_params;
        object_params = nullptr;
    }
}
