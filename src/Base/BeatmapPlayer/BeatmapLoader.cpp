#include "BeatmapLoader.h"

#include <cstdio>

#include "BeatmapPlayer.h"
#include "../SceneManagement/SceneManager.h"

BeatmapLoader *BeatmapLoader::Init(BeatmapPlayer *player) {
    if (player == nullptr) return nullptr;
    //TODO: check if ruleset and beatmaps are correct (they should be if one of the other Init function were used)
    return new BeatmapLoader(player);
}


BeatmapLoader::BeatmapLoader(BeatmapPlayer *player) : m_player(player), m_status(StatusNotLoaded) {
}

BeatmapLoader::~BeatmapLoader() {
    delete m_player;
}


void BeatmapLoader::Update(float dt) {
    Scene::Update(dt);

    //We are doing things like this because we want the first frame of gameplay to happen after the frame where the
    //resources are loaded (because if it takes a long time to load resources the value of DT of the first gameplay
    //frame could be big)
    //Rework this when this gets multi-threaded lol

    switch (m_status) {
        case StatusNotLoaded:
            //TODO: move this to another thread
            m_status = m_player->LoadResources() ? StatusLoaded : StatusError;
            break;

        case StatusLoading:
            //TODO: add stuff here when the process is eventually multithreaded?
            break;

        case StatusLoaded:
            m_player->OnGameStart();
            GetManager()->SetScene(m_player);
            m_player = nullptr; //we don't want to deallocate the player after setting it as the current scene
            break;

        case StatusError:
            //TODO: display meaningful error? just yell in the console for now
            //      also go back to the main menu when there is one
            printf("BeatmapLoader error\n");
            break;
    }
}

void BeatmapLoader::Draw() {
    Scene::Draw();

    //Since this is currently single-threaded, this means it is basically useless to draw anything in here.
    //In the future maybe we could display information about the map being loaded
    //(But that shouldn't really mater since almost all maps load instantly)

    //TODO (psp): because we can't see any debug message, make sure an eventual error while loading would appear
    //            on screen
}

