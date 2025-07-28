#ifndef FILEFORMATENTRY_H
#define FILEFORMATENTRY_H


class Ruleset;
class Beatmap;


enum FileFormat {
    UnknownFormat = 0,
    OsuTaikoBeatmap,
};

//FIXME: bad naming, but idk
struct FileFormatEntry {
    //Should return nullptr if the fileformat is incorrect or if the beatmap isn't parsed correctly
    Beatmap *(*parse_beatmap)(const char *);

    //Returns the ruleset corresponding to the file format
    Ruleset *(*make_ruleset)();

    //Unique value identifying the file format/ruleset pair
    FileFormat type;

    //Will be used in the future to display the type of the map in the main menu
    const char *ruleset_name;
};



#endif //FILEFORMATENTRY_H
