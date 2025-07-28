#ifndef FILEFORMATREGISTRY_RAYLIB_H
#define FILEFORMATREGISTRY_RAYLIB_H

#include "FileFormatEntry.h"

#include <cstddef>

//Must be implemented per-platform
const FileFormatEntry *GetFileFormatEntries();
size_t GetFileFormatCount();

//Platform independent
const FileFormatEntry *GetFileFormatEntry(FileFormat type);
const FileFormatEntry *MatchFileFormatFromFilename(const char *filename, Beatmap **beatmap_out = nullptr);



#endif //FILEFORMATREGISTRY_RAYLIB_H
