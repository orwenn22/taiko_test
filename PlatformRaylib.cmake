set(CMAKE_CXX_STANDARD 20)

file(COPY res_raylib/ DESTINATION res)

add_subdirectory(external/raylib)

add_executable(${PROJECT_NAME} src/Raylib/main.cpp
        src/Base/Beatmap/OsuBeatmap/OsuBeatmap.cpp
        src/Base/Beatmap/OsuBeatmap/OsuBeatmap.h
        src/Base/Beatmap/TaikoBeatmap/TaikoHit.h
        src/Base/Utils.h
        src/Base/Beatmap/OsuBeatmap/OsuHitObject.cpp
        src/Base/Beatmap/OsuBeatmap/OsuHitObject.h
        src/Base/Utils.cpp
        src/Base/Beatmap/OsuBeatmap/OsuTimingPoint.cpp
        src/Base/Beatmap/OsuBeatmap/OsuTimingPoint.h
        src/Base/Beatmap/TaikoBeatmap/TaikoBeatmap.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoBeatmap.h
        src/Base/Beatmap/TaikoBeatmap/TaikoHit.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoEffectPoint.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoEffectPoint.h
        src/Base/BeatmapPlayer/BeatmapPlayer.cpp
        src/Base/BeatmapPlayer/BeatmapPlayer.h
        src/Base/Ruleset/Ruleset.cpp
        src/Base/Ruleset/Ruleset.h
        src/Base/Beatmap/Beatmap.cpp
        src/Base/Beatmap/Beatmap.h
        src/Base/Ruleset/TaikoRuleset/TaikoRuleset.cpp
        src/Base/Ruleset/TaikoRuleset/TaikoRuleset.h
        src/Raylib/Input/InputHandling_Multithread.cpp
        src/Raylib/Input/InputHandling_Singlethread.cpp
        src/Base/Input/InputHandling.h
        src/Raylib/main.h
        src/Raylib/Config.h
        src/Time.cpp
        src/Time.h
        src/Base/Beatmap/TaikoBeatmap/TaikoTimingPoint.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoTimingPoint.h
        src/Base/Beatmap/TaikoBeatmap/TaikoDrumRoll.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoDrumRoll.h
        src/Raylib/Ruleset/TaikoRuleset/TaikoRulesetRaylib.cpp
        src/Raylib/Ruleset/TaikoRuleset/TaikoRulesetRaylib.h
        src/Raylib/UtilsRaylib.cpp
        src/Raylib/UtilsRaylib.h
        src/Raylib/BeatmapPlayer/BeatmapPlayerRaylib.cpp
        src/Raylib/BeatmapPlayer/BeatmapPlayerRaylib.h
        src/Base/SceneManagement/SceneManager.cpp
        src/Base/SceneManagement/SceneManager.h
        src/Base/SceneManagement/Scene.cpp
        src/Base/SceneManagement/Scene.h
        src/Base/MapIndex/MapIndexFile.cpp
        src/Base/MapIndex/MapIndexFile.h
        src/Base/Registry/FileFormat/FileFormatEntry.h
        src/Raylib/Registry/FileFormat/FileFormatRegistry_raylib.cpp
        src/Base/Registry/FileFormat/FileFormatRegistry.h
        src/Base/Registry/FileFormat/FileFormatRegistry.cpp
        src/Base/Beatmap/OsuBeatmap/OsuBeatmapParsing.cpp
        src/Base/BeatmapPlayer/BeatmapLoader.cpp
        src/Base/BeatmapPlayer/BeatmapLoader.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
