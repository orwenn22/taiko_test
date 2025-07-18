#ifndef OSUHITOBJECT_H
#define OSUHITOBJECT_H

#include <cstddef>

enum OsuHitObjectType {
    HitCircle = 0b00000001,
    Slider = 0b00000010,
    NewCombo = 0b00000100,
    Spinner = 0b00001000,
};

enum OsuHitSounds {
    Normal = 0b00000001,  //Doesn't change anything to the color of the note in taiko (in lazer's editor "new combo" enables this?)
    Whistle = 0b00000010,  //Make the note blue in taiko
    Finish = 0b00000100,  //Make the note strong in taiko
    Clap = 0b00001000,  //Make the note blue in taiko
};

//Dummy object to make all the other ones innerit from it (this is just to avoid using a void *)
struct OsuHitObjectParams {
    virtual ~OsuHitObjectParams();
};


struct CurvePoint {
    float x, y;
};

//https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#sliders
struct OsuSliderHitObjectParams : OsuHitObjectParams {
    //Type of curve used to construct this slider (B = bézier, C = centripetal catmull-rom, L = linear, P = perfect circle)
    char curve_type;

    // (Pipe-separated list of strings): Anchor points used to construct the slider. Each point is in the format x:y
    CurvePoint *curve_points;
    size_t curve_point_count;

    int slides; //Amount of times the player has to follow the slider's curve back-and-forth before the slider is complete.
    float length; //Visual length in osu! pixels of the slider.

    int *edge_sounds;
    size_t edge_sound_count;

    //TODO : add edgeSets?

    OsuSliderHitObjectParams();
    ~OsuSliderHitObjectParams() override;


    // Notice: The slider's length can be used to determine the time it takes to complete the slider.
    // length / (SliderMultiplier * 100 * SV) * beatLength tells how many milliseconds it takes to complete one slide
    // of the slider (where SV is the slider velocity multiplier given by the effective inherited timing point, or 1
    // if there is none).
};

//https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#spinners
struct OsuSpinnerHitObjectParams : OsuHitObjectParams {
    int end_time; //End time of the spinner, in milliseconds from the beginning of the beatmap's audio.

    OsuSpinnerHitObjectParams(int end_time = 0);
    ~OsuSpinnerHitObjectParams() override;
};

//https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#hit-objects
struct OsuHitObject {
    int x, y, time;
    int type;
    int hitsound;
    OsuHitObjectParams *object_params;
    //ignore hit samples

    OsuHitObject(int x, int y, int time, int type, int hitsound);
    ~OsuHitObject();

    [[nodiscard]] bool IsOfType(OsuHitObjectType osu_type) const { return (type&osu_type) != 0; }
};



#endif //OSUHITOBJECT_H
