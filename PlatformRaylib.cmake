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
        src/BeatmapPlayer.cpp
        src/BeatmapPlayer.h
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
        src/Platform/Raylib/UtilsRaylib.h)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
