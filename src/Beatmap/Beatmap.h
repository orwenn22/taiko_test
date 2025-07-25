#ifndef BEATMAP_H
#define BEATMAP_H



class Beatmap {
public:
    Beatmap();
    ~Beatmap();

    //The root path should be used to load resources bundled with the map (song, images (?), etc)
    const char *GetRootPath();
    void SetRootPath(const char *path);
    void SetRootPathFromFile(const char *file_path); //this basically strips whatever is after the last '/' of the path

private:
    char *m_root_path;
};



#endif //BEATMAP_H
