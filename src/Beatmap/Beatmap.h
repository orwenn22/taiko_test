#ifndef BEATMAP_H
#define BEATMAP_H



class Beatmap {
public:
    Beatmap();
    virtual ~Beatmap();

    //The root path should be used to load resources bundled with the map (song, images (?), etc)
    const char *GetRootPath();
    void SetRootPath(const char *path);
    void SetRootPathFromFile(const char *file_path); //this basically strips whatever is after the last '/' of the path

    void SetBackground(const char *background_path); //background_path relative to root path
    const char *GetBackground();

private:
    char *m_root_path; //heap allocated
    char *m_background_path; //heap allocated, this is relative to the root path
};



#endif //BEATMAP_H
