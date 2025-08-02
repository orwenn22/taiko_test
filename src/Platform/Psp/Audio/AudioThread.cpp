#include "AudioThread.h"

#include <cstring>

#include "resample.h"

#define TEMP_BUFFER_SAMPLES 4096 // stereo samples (2048 frames)
static short temp_decode_buffer[TEMP_BUFFER_SAMPLES]; // holds raw decoded samples before resampling them

//TODO: if there are weird audio bugs in the future, make this volatile and manually cast it everywhere to AudioStream *
//      or use a mutex? might be overkill idk
static AudioStream *s_current_stream = nullptr;


void AudioCallback(void* buf, unsigned int length, void *userdata) {
    if (s_current_stream == nullptr || s_current_stream->IsPaused()) return;

    int source_sample_rate = s_current_stream->GetSampleRate();
    short *out = (short *)buf; //anything we put here will be played

    if (source_sample_rate == PSP_SAMPLE_RATE) {
        //no resampling needed
        int decoded = s_current_stream->Decode(out, length);

        //fill remaining space with 0s
        if (decoded < length) {
            memset(out + decoded*2, 0, (length - decoded) * 2 * sizeof(short));
        }
    }
    else {
        //resampling needed
        float ratio = (float)source_sample_rate / (float)PSP_SAMPLE_RATE;
        int needed_input_frames = (int)(length * ratio) + 1; // +1 for safety

        if (needed_input_frames * 2 > TEMP_BUFFER_SAMPLES) {
            needed_input_frames = TEMP_BUFFER_SAMPLES / 2; // clamp to avoid overflow
        }

        int decoded_frames = s_current_stream->Decode(temp_decode_buffer, needed_input_frames);

        int resampled_frames = resample_linear(
            temp_decode_buffer,
            decoded_frames,
            out,
            length,
            2,         // stereo
            ratio
        );

        //fill remaining space with 0s
        if (resampled_frames < (int)length) {
            memset(out + resampled_frames * 2, 0, (length - resampled_frames) * 2 * sizeof(short));
        }

    }
}

void SetCurrentAudioStream(AudioStream* stream) {
    s_current_stream = stream;
}

AudioStream *GetCurrentAudioStream() {
    return s_current_stream;
}
