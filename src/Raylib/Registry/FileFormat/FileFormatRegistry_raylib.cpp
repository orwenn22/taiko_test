#include "../../../Base/Registry/FileFormat/FileFormatRegistry.h"

#include "../../../Base/Registry/FileFormat/FileFormatEntry.h"
#include "../../../Base/Beatmap/TaikoBeatmap/TaikoBeatmap.h"
#include "../../Ruleset/TaikoRuleset/TaikoRulesetRaylib.h"

const FileFormatEntry s_file_formats_raylib[] = {
    {
        (Beatmap *(*)(const char *))TaikoBeatmap::FromOsuBeatmapPath,
        (Ruleset *(*)())TaikoRulesetRaylib::build,
        OsuTaikoBeatmap,
        "osu!taiko"
    },
};
const size_t s_file_formats_array_size = sizeof(s_file_formats_raylib) / sizeof(FileFormatEntry);


const FileFormatEntry *GetFileFormatEntries() {
    return s_file_formats_raylib;
}

size_t GetFileFormatCount() {
    return s_file_formats_array_size;
}

