#include "Beatmap.h"

#include <cstdlib>
#include <cstring>

Beatmap::Beatmap() : m_root_path(nullptr), m_background_path(nullptr) {}

Beatmap::~Beatmap() {
    if (m_root_path) free(m_root_path);
    m_root_path = nullptr;

    if (m_background_path) free(m_background_path);
    m_background_path = nullptr;
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


void Beatmap::SetBackground(const char *background_path) {
    if (background_path == nullptr) return;
    if (m_background_path != nullptr) free(m_background_path);
    m_background_path = strdup(background_path);
}

const char *Beatmap::GetBackground() {
    return m_background_path;
}
