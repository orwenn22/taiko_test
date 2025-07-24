#ifndef AUDIOSTREAMMP3_H
#define AUDIOSTREAMMP3_H

#include "AudioStream.h"

#include <minimp3.h>
#include <minimp3_ex.h>

class AudioStreamMP3 : public AudioStream {
public:
    static AudioStreamMP3 *init(const char *filename);
    ~AudioStreamMP3() override;


    int Decode(short *output, int frames) override;
    int Seek(float seconds) override;
    float GetDuration() override;
    int GetSampleRate() override;

private:
    AudioStreamMP3();

    mp3dec_ex_t m_mp3;
};



#endif //AUDIOSTREAMMP3_H
