#ifndef BEATMAPLOADER_H
#define BEATMAPLOADER_H

#include "BeatmapPlayer.h"
#include "../SceneManagement/Scene.h"
#include "../Registry/FileFormat/FileFormatEntry.h"
#include "../Registry/FileFormat/FileFormatRegistry.h"


class Beatmap;
class Ruleset;

enum BeatmapLoaderStatus {
    StatusNotLoaded,
    StatusLoading, //when we eventually load on another thread, this will be used to indicate to the main thread that we are currently loading stuff
    StatusLoaded,
    StatusError
};

//the goal of this is to make sure all the resources of the maps are loaded before entering gameplay.
//despite its (bad) name, it doesn't actually load the map, it loads its resources (background, etc) and the ruleset's
//resources.
class BeatmapLoader : public Scene {
public:
    //make a loader from a BeatmapPlayer
    static BeatmapLoader *Init(BeatmapPlayer *player);

    //make a loader from a ruleset and beatmap
    template <typename BeatmapPlayerType>
    static BeatmapLoader *Init(Ruleset *ruleset, Beatmap *beatmap) {
        static_assert(std::is_base_of_v<BeatmapPlayer, BeatmapPlayerType>, "invalid BeatmapPlayerType");

        if (ruleset == nullptr || beatmap == nullptr) return nullptr;
        return Init(new BeatmapPlayerType(ruleset, beatmap));
    }

    //load a beatmap from a filename.
    template <typename BeatmapPlayerType>
    static BeatmapLoader *LoadFromFile(const char *filename, const FileFormatEntry *file_format = nullptr) {
        static_assert(std::is_base_of_v<BeatmapPlayer, BeatmapPlayerType>, "invalid BeatmapPlayerType");
        if (filename == nullptr) return nullptr;

        //actually try to load the map
        //TODO: maybe this should be moved to somewhere else
        Beatmap *beatmap = nullptr;
        if (file_format == nullptr) file_format = MatchFileFormatFromFilename(filename, &beatmap);
        else beatmap = file_format->parse_beatmap(filename);

        if (file_format == nullptr || beatmap == nullptr) return nullptr;

        //make BeatmapLoader to load resources
        return Init<BeatmapPlayerType>(file_format->make_ruleset(), beatmap);
    }

private:
    BeatmapLoader(BeatmapPlayer *player);

public:
    ~BeatmapLoader() override;

    void Update(float dt) override;
    void Draw() override;

private:
    BeatmapPlayer *m_player;
    BeatmapLoaderStatus m_status;
};



#endif //BEATMAPLOADER_H
