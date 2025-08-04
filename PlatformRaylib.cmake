set(CMAKE_CXX_STANDARD 20)

file(COPY res_raylib/ DESTINATION res)

add_subdirectory(external/raylib)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Beatmap/OsuBeatmap/OsuBeatmap.cpp
        src/Beatmap/OsuBeatmap/OsuBeatmap.h
        src/Beatmap/TaikoBeatmap/TaikoHit.h
        src/Utils.h
        src/Beatmap/OsuBeatmap/OsuHitObject.cpp
        src/Beatmap/OsuBeatmap/OsuHitObject.h
        src/Utils.cpp
        src/Beatmap/OsuBeatmap/OsuTimingPoint.cpp
        src/Beatmap/OsuBeatmap/OsuTimingPoint.h
        src/Beatmap/TaikoBeatmap/TaikoBeatmap.cpp
        src/Beatmap/TaikoBeatmap/TaikoBeatmap.h
        src/Beatmap/TaikoBeatmap/TaikoHit.cpp
        src/Beatmap/TaikoBeatmap/TaikoEffectPoint.cpp
        src/Beatmap/TaikoBeatmap/TaikoEffectPoint.h
        src/BeatmapPlayer/BeatmapPlayer.cpp
        src/BeatmapPlayer/BeatmapPlayer.h
        src/Ruleset/Ruleset.cpp
        src/Ruleset/Ruleset.h
        src/Beatmap/Beatmap.cpp
        src/Beatmap/Beatmap.h
        src/Ruleset/TaikoRuleset/TaikoRuleset.cpp
        src/Ruleset/TaikoRuleset/TaikoRuleset.h
        src/Input/InputHandling_Multithread.cpp
        src/Input/InputHandling_Singlethread.cpp
        src/Input/InputHandling.h
        src/main.h
        src/Config.h
        src/Time.cpp
        src/Time.h
        src/Beatmap/TaikoBeatmap/TaikoTimingPoint.cpp
        src/Beatmap/TaikoBeatmap/TaikoTimingPoint.h
        src/Beatmap/TaikoBeatmap/TaikoDrumRoll.cpp
        src/Beatmap/TaikoBeatmap/TaikoDrumRoll.h
        src/Ruleset/TaikoRuleset/TaikoRulesetRaylib.cpp
        src/Ruleset/TaikoRuleset/TaikoRulesetRaylib.h
        src/Platform/Raylib/UtilsRaylib.cpp
        src/Platform/Raylib/UtilsRaylib.h
        src/BeatmapPlayer/BeatmapPlayerRaylib.cpp
        src/BeatmapPlayer/BeatmapPlayerRaylib.h
        src/SceneManagement/SceneManager.cpp
        src/SceneManagement/SceneManager.h
        src/SceneManagement/Scene.cpp
        src/SceneManagement/Scene.h
        src/MapIndex/MapIndexFile.cpp
        src/MapIndex/MapIndexFile.h
        src/Registry/FileFormat/FileFormatEntry.h
        src/Registry/FileFormat/FileFormatRegistry_raylib.cpp
        src/Registry/FileFormat/FileFormatRegistry.h
        src/Registry/FileFormat/FileFormatRegistry.cpp
        src/Beatmap/OsuBeatmap/OsuBeatmapParsing.cpp
        src/BeatmapPlayer/BeatmapLoader.cpp
        src/BeatmapPlayer/BeatmapLoader.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
