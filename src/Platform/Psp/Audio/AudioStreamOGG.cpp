#include "AudioStreamOGG.h"

//lol
#ifdef STB_VORBIS_HEADER_ONLY
#undef STB_VORBIS_HEADER_ONLY
#endif
//#define STB_VORBIS_NO_STDIO
#define STB_VORBIS_NO_PUSHDATA_API
#define STB_VORBIS_MAX_CHANNELS 2
#include <stb_vorbis.c>

AudioStreamOGG * AudioStreamOGG::init(const char *filename) {
    if (filename == nullptr) return nullptr;

    auto result = new AudioStreamOGG();

    int error;
    result->m_ogg = stb_vorbis_open_filename(filename, &error, nullptr);

    if (result->m_ogg == nullptr) {
        delete result;
        return nullptr;
    }

    return result;
}

AudioStreamOGG::AudioStreamOGG() : m_ogg(nullptr) {

}

AudioStreamOGG::~AudioStreamOGG() {
    stb_vorbis_close(m_ogg);
}

int AudioStreamOGG::Decode(short *output, int frames) {
    int samples = stb_vorbis_get_samples_short_interleaved(m_ogg, 2, output, frames * 2);
    return samples;
}

int AudioStreamOGG::Seek(float seconds) {
    m_paused = true; //pausing prevent artefacts from being heard while seeking

    if (seconds < 0.f) seconds = 0.f;
    int target_frame = (int)(seconds * (float) m_ogg->sample_rate);

    int result = !stb_vorbis_seek_frame(m_ogg, target_frame);
    m_paused = false;
    return result ? 0 : 1;
}

float AudioStreamOGG::GetDuration() {
    return stb_vorbis_stream_length_in_seconds(m_ogg);
}

int AudioStreamOGG::GetSampleRate() {
    return (int) m_ogg->sample_rate;
}
