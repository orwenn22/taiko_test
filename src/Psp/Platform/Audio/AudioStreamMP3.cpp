#include "AudioStreamMP3.h"

#define MINIMP3_ONLY_MP3
#define MINIMP3_NO_SIMD
#define MINIMP3_IMPLEMENTATION
#include <minimp3_ex.h>

AudioStreamMP3 *AudioStreamMP3::init(const char *filename) {
    auto result = new AudioStreamMP3;
    int return_code = mp3dec_ex_open(&result->m_mp3, filename, MP3D_SEEK_TO_SAMPLE);

    if (return_code != 0) {
        delete result;
        return nullptr;
    }

    return result;
}

AudioStreamMP3::AudioStreamMP3() : m_mp3{} {}

AudioStreamMP3::~AudioStreamMP3() {
    mp3dec_ex_close(&m_mp3);
}

int AudioStreamMP3::Decode(short *output, int frames) {
    int samples = frames * 2;
    return (int) (mp3dec_ex_read(&m_mp3, output, samples) / 2);
}

int AudioStreamMP3::Seek(float seconds) {
    m_paused = true; //pausing prevent artefacts from being heard while seeking

    if (seconds < 0.f) seconds = 0.f;
    else if (seconds >= GetDuration()) seconds = 0.f; //go back to the start for now? idk

    uint64_t sample_offset = (uint64_t)(seconds * m_mp3.info.hz);
    int result = (mp3dec_ex_seek(&m_mp3, sample_offset) == 0);
    m_paused = false;
    return result;
}

float AudioStreamMP3::GetDuration() {
    return (float) m_mp3.samples / (float) m_mp3.info.hz;
}

int AudioStreamMP3::GetSampleRate() {
    return m_mp3.info.hz;
}
