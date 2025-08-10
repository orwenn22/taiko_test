#ifndef AUDIOSTREAMOGG_H
#define AUDIOSTREAMOGG_H

#include "AudioStream.h"

#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

class AudioStreamOGG : public AudioStream {
public:
    static AudioStreamOGG *init(const char *filename);
    ~AudioStreamOGG() override;


    int Decode(short *output, int frames) override;
    int Seek(float seconds) override;
    float GetDuration() override;
    int GetSampleRate() override;

private:
    AudioStreamOGG();

    stb_vorbis *m_ogg;
};



#endif //AUDIOSTREAMOGG_H
