#ifndef RESAMPLE_H
#define RESAMPLE_H

#define PSP_SAMPLE_RATE 44100

int resample_linear(
    const short *in,       // input PCM
    int in_samples,        // number of stereo frames
    short *out,            // output buffer
    int out_samples,       // desired output stereo frames
    int channels,          // 1 = mono, 2 = stereo
    float ratio            // input_rate / output_rate
);

#endif //RESAMPLE_H
