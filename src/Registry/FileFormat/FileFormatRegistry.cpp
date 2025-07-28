#include "FileFormatRegistry.h"

#include "../../Beatmap/Beatmap.h"

const FileFormatEntry *GetFileFormatEntry(FileFormat type) {
    const FileFormatEntry *file_formats = GetFileFormatEntries();
    size_t file_format_count = GetFileFormatCount();

    for (size_t i = 0; i < file_format_count; ++i) {
        if (file_formats[i].type == type) return &file_formats[i];
    }
    return nullptr;
}

//FIXME: maybe the same map file could be played by multiple ruleset in the future, similar to osu lazer
//FIXME 2: maybe we should be able to also return the map?
const FileFormatEntry *MatchFileFormatFromFilename(const char *filename, Beatmap **beatmap_out) {
    if (filename == nullptr) return nullptr;
    const FileFormatEntry *file_formats_array_size = GetFileFormatEntries();
    size_t file_format_count = GetFileFormatCount();

    for (size_t i = 0; i < file_format_count; ++i) {
        Beatmap *beatmap = file_formats_array_size[i].parse_beatmap(filename);
        if (beatmap == nullptr) continue;

        if (beatmap_out == nullptr) delete beatmap;
        else *beatmap_out = beatmap;

        return &file_formats_array_size[i];
    }

    return nullptr; //couldn't match file to anything
}
