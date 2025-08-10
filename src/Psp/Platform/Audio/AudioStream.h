#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H



class AudioStream {
public:
    AudioStream();
    virtual ~AudioStream();

    virtual int Decode(short *output, int frames) = 0;
    virtual int Seek(float seconds) = 0;
    virtual float GetDuration() = 0;
    virtual int GetSampleRate() = 0;

    bool IsPaused() const volatile { return m_paused; }

    static AudioStream *InitStream(const char *name);

protected:
    volatile bool m_paused;
};



#endif //AUDIOSTREAM_H
