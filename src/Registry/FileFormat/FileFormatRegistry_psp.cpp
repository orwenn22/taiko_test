#include "FileFormatRegistry.h"

#include "FileFormatEntry.h"
#include "../../Beatmap/TaikoBeatmap/TaikoBeatmap.h"
#include "../../Ruleset/TaikoRuleset/TaikoRulesetPsp.h"

const FileFormatEntry s_file_formats_psp[] = {
    {
        (Beatmap *(*)(const char *))TaikoBeatmap::FromOsuBeatmapPath,
        (Ruleset *(*)())TaikoRulesetPsp::build,
        OsuTaikoBeatmap,
        "osu!taiko"
    },
};
const size_t s_file_formats_array_size = sizeof(s_file_formats_psp) / sizeof(FileFormatEntry);


const FileFormatEntry *GetFileFormatEntries() {
    return s_file_formats_psp;
}

size_t GetFileFormatCount() {
    return s_file_formats_array_size;
}
