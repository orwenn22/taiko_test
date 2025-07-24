#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H



class AudioStream {
public:
    virtual ~AudioStream();

    virtual int Decode(short *output, int frames) = 0;
    virtual int Seek(float seconds) = 0;
    virtual float GetDuration() = 0;
    virtual int GetSampleRate() = 0;

    static AudioStream *InitStream(const char *name);
};



#endif //AUDIOSTREAM_H
