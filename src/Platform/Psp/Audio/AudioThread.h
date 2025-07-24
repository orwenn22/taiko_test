#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include "AudioStream.h"

void AudioCallback(void* buf, unsigned int length, void *userdata);

void SetCurrentAudioStream(AudioStream* stream);
AudioStream *GetCurrentAudioStream();

#endif //AUDIOTHREAD_H
