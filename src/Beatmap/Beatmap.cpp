#include "Beatmap.h"

#include <cstdlib>
#include <string.h>

Beatmap::Beatmap() : m_root_path(nullptr) {}

Beatmap::~Beatmap() {
    if (m_root_path) free(m_root_path);
    m_root_path = nullptr;
}

const char *Beatmap::GetRootPath() {
    if (m_root_path == nullptr) return ""; //empty string
    return m_root_path;
}

void Beatmap::SetRootPath(const char *path) {
    if (path == nullptr) return;
    m_root_path = strdup(path);
}

void Beatmap::SetRootPathFromFile(const char *file_path) {
    if (file_path == nullptr) return;

    //find last /
    size_t len = strlen(file_path);
    size_t last_slash = len;
    for (size_t i = 0; i < len; ++i) {
        if (file_path[i] == '/') last_slash = i;
    }

    //no '/' in path
    if (last_slash == len) {
        SetRootPath("./"); //idk let's assume this is the current directory then
        return;
    }

    if (m_root_path) free(m_root_path);
    m_root_path = (char *) malloc(sizeof(char) * (last_slash + 2));
    strncpy(m_root_path, file_path, last_slash + 1);
    m_root_path[last_slash + 1] = 0; //null termination
}
