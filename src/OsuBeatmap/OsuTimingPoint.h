#ifndef OSUTIMINGPOINT_H
#define OSUTIMINGPOINT_H

// Set the scroll speed to 2 : 4065.367089002009,-50,3,1,0,100,0,0  (-50 and uninherited set to 0)

enum OsuTimingPointEffect {
    Kiai = 0b00000001,
    OmitFirstBarLine = 0b00001000,
};

struct OsuTimingPoint {
    //time (Integer): Start time of the timing section, in milliseconds from the beginning of the beatmap's audio.
    //The end of the timing section is the next timing point's time (or never, if this is the last timing point).
    int time;

    //beatLength (Decimal): This property has two meanings:
    //For uninherited timing points, the duration of a beat, in milliseconds.
    //For inherited timing points, a negative inverse slider velocity multiplier, as a percentage.
    //For example, -50 would make all sliders in this timing section twice as fast as SliderMultiplier (beatmap property)
    float beat_length;

    //meter (Integer): Amount of beats in a measure. Inherited timing points ignore this property
    //"time signature" in lazer's editor
    int meter;

    //sampleSet (Integer): Default sample set for hit objects (0 = beatmap default, 1 = normal, 2 = soft, 3 = drum).
    int sample_set;

    //sampleIndex (Integer): Custom sample index for hit objects. 0 indicates osu!'s default hitsounds.
    int sample_index;

    //volume (Integer): Volume percentage for hit objects.
    int volume;

    //uninherited (0 or 1): Whether or not the timing point is uninherited.
    //(if this is 1 beat_length changes the bpm, if this is 0 beat_length changes the scroll speed)
    bool uninherited;

    //effects (Integer): Bit flags that give the timing point extra effects. (https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#effects)
    int effects;

    OsuTimingPoint();
    OsuTimingPoint(int time, float beat_length, int meter, int sample_set, int sample_index, int volume, bool uninherited, int effects);
    ~OsuTimingPoint();

    float GetScrollSpeedMultiplier();
};

//should check if beat_length is negative before calling that
float BeatlengthToScrollMultiplier(float beat_length);

#endif //OSUTIMINGPOINT_H
