#include "AudioStream.h"

#include <cstring>

#include "AudioStreamMP3.h"
#include "AudioStreamOGG.h"
#include "AudioThread.h"

AudioStream::~AudioStream() {
    if (GetCurrentAudioStream() == this) SetCurrentAudioStream(nullptr);
}

AudioStream *AudioStream::InitStream(const char *filename) {
    if (filename == nullptr) return nullptr;

    //try to match the filename using its extension
    const char *ext = strrchr(filename, '.'); //find dot (start of extension)
    if (!ext) return nullptr;

    while (*ext == '.') ++ext; // skip dot(s)

    if (strcmp(ext, "mp3") == 0) {
        return AudioStreamMP3::init(filename);
    }
    else if (strcmp(ext, "ogg") == 0) {
        return AudioStreamOGG::init(filename);
    }
    else {
        return nullptr; //anything else is unsupported
    }
}
