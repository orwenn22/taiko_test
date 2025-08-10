#include "MapIndexFile.h"

#include <cstdio>
#include <cstring>
#include <dirent.h>

#include "../Utils.h"
#include "../Registry/FileFormat/FileFormatRegistry.h"

bool MapIndexExist(const char *maps_root_path) {
    if (maps_root_path == nullptr) return false;

    char mapindex_path[PATH_MAX];
    snprintf(mapindex_path, PATH_MAX, "%s/%s", maps_root_path, maps_root_path);
    FILE *file = fopen("res/mapindex.txt", "r");
    if (file == NULL) return false;
    fclose(file);
    return true;
}


// out_file : mapindex.txt, containing a list of all the maps
// subdir_path : full or relative path to a map directory
// subdir_name : name of the directory, as it is written in mapindex_dirs.txt
static void HandleMapSubdirectory(FILE *out_file, const char *subdir_path, const char *subdir_name) {
    printf("Scanning subdirectory %s\n", subdir_path);

    //open map folder
    DIR *map_subdir = opendir(subdir_path);

    //create map index file inside map folder
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", subdir_path, "mapindex.txt");
    FILE *mapindex_file = fopen(path, "w");

    for (dirent *entry = readdir(map_subdir); entry != nullptr; entry = readdir(map_subdir)) {
        if (entry->d_type != DT_REG) continue; //exclude non-regular files
        if (EndsWith(entry->d_name, ".mp3") || EndsWith(entry->d_name, ".ogg")) continue; //exclude music files

        //get the full path of the map directory
        snprintf(path, sizeof(path), "%s/%s", subdir_path, entry->d_name);

        //identify file
        const FileFormatEntry *file_format = MatchFileFormatFromFilename(path);
        if (file_format == nullptr) printf("Couldn't match %s to anything :(\n", entry->d_name);
        else {
            printf("Matched %s to %s", entry->d_name, file_format->ruleset_name);

            fprintf(mapindex_file, "%i %s\n", file_format->type, entry->d_name);
            fprintf(out_file, "%i %s/%s\n", file_format->type, subdir_name, entry->d_name);
        }
    }

    fclose(mapindex_file);
    closedir(map_subdir);
}

bool GenerateMapIndexFile(const char *maps_root_path) {
    if (maps_root_path == nullptr) return false;

    //{maps_root_path}/mapindex_dirs.txt
    char mapindex_dirs_path[PATH_MAX];
    snprintf(mapindex_dirs_path, sizeof(mapindex_dirs_path), "%s/%s", maps_root_path, "mapindex_dirs.txt");

    //{maps_root_path}/mapindex.txt
    char mapindex_path[PATH_MAX];
    snprintf(mapindex_path, sizeof(mapindex_path), "%s/%s", maps_root_path, "mapindex.txt");

    //write first mapindex file
    FILE *mapindex_dirs_file = fopen(mapindex_dirs_path, "w");
    if (mapindex_dirs_file == nullptr) return false;

    //open map directory
    DIR *maps_directory = opendir(maps_root_path);
    if (maps_directory == nullptr) return false;

    for (dirent *entry = readdir(maps_directory); entry != nullptr; entry = readdir(maps_directory)) {
        if (entry->d_type != DT_DIR) continue; //ignore non-directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue; //ignore . and ..

        //write in the map index the name of all the folders in map_directory
        fprintf(mapindex_dirs_file, "%s\n", entry->d_name);
    }

    closedir(maps_directory);
    fclose(mapindex_dirs_file);


    //read back from it, and make sub-indexes
    mapindex_dirs_file = fopen(mapindex_dirs_path, "r");
    if (mapindex_dirs_file == nullptr) return false;

    FILE *mapindex_file = fopen(mapindex_path, "w");
    if (mapindex_file == nullptr) return false;

    char line[PATH_MAX]; //store the lines of the mapindex fime
    char subdir_path[PATH_MAX]; //store the path to the subdirectories
    while (fgets(line, sizeof(line), mapindex_dirs_file) != nullptr) {
        RemoveNewline(line);
        snprintf(subdir_path, sizeof(subdir_path), "%s/%s",maps_root_path, line);
        HandleMapSubdirectory(mapindex_file, subdir_path, line);
    }
    fclose(mapindex_dirs_file);
    return true;
}
