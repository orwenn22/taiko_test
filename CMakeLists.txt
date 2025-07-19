cmake_minimum_required(VERSION 3.31)
project(taiko_test)

set(CMAKE_CXX_STANDARD 20)

file(COPY res DESTINATION .)

add_subdirectory(external/raylib)

add_executable(taiko_test src/main.cpp
        src/OsuBeatmap/OsuBeatmap.cpp
        src/OsuBeatmap/OsuBeatmap.h
        src/TaikoBeatmap/TaikoHit.h
        src/Utils.h
        src/OsuBeatmap/OsuHitObject.cpp
        src/OsuBeatmap/OsuHitObject.h
        src/Utils.cpp
        src/OsuBeatmap/OsuTimingPoint.cpp
        src/OsuBeatmap/OsuTimingPoint.h
        src/TaikoBeatmap/TaikoBeatmap.cpp
        src/TaikoBeatmap/TaikoBeatmap.h
        src/TaikoBeatmap/TaikoHit.cpp
        src/TaikoBeatmap/TaikoEffectPoint.cpp
        src/TaikoBeatmap/TaikoEffectPoint.h
        src/BeatmapPlayer.cpp
        src/BeatmapPlayer.h
        src/Ruleset.cpp
        src/Ruleset.h
        src/Beatmap.cpp
        src/Beatmap.h
        src/TaikoRuleset.cpp
        src/TaikoRuleset.h
        src/InputHandling_Multithread.cpp
        src/InputHandling_Singlethread.cpp
        src/InputHandling.h
        src/main.h
        src/Config.h
        src/Time.cpp
        src/Time.h
        src/TaikoBeatmap/TaikoTimingPoint.cpp
        src/TaikoBeatmap/TaikoTimingPoint.h
        src/TaikoBeatmap/TaikoDrumRoll.cpp
        src/TaikoBeatmap/TaikoDrumRoll.h)

target_link_libraries(taiko_test PRIVATE raylib)
