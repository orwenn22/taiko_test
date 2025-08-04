set(CMAKE_CXX_STANDARD 20)

file(COPY res_psp/ DESTINATION res)


add_executable(${PROJECT_NAME} src/main_Psp.cpp
        src/Utils.h
        src/Utils.cpp
        src/Beatmap/Beatmap.cpp
        src/Beatmap/Beatmap.h
        src/Beatmap/OsuBeatmap/OsuBeatmap.cpp
        src/Beatmap/OsuBeatmap/OsuBeatmap.h
        src/Beatmap/OsuBeatmap/OsuHitObject.cpp
        src/Beatmap/OsuBeatmap/OsuHitObject.h
        src/Beatmap/OsuBeatmap/OsuTimingPoint.cpp
        src/Beatmap/OsuBeatmap/OsuTimingPoint.h
        src/Beatmap/TaikoBeatmap/TaikoHit.cpp
        src/Beatmap/TaikoBeatmap/TaikoHit.h
        src/Beatmap/TaikoBeatmap/TaikoEffectPoint.cpp
        src/Beatmap/TaikoBeatmap/TaikoEffectPoint.h
        src/Beatmap/TaikoBeatmap/TaikoBeatmap.cpp
        src/Beatmap/TaikoBeatmap/TaikoBeatmap.h
        src/Beatmap/TaikoBeatmap/TaikoTimingPoint.cpp
        src/Beatmap/TaikoBeatmap/TaikoTimingPoint.h
        src/Beatmap/TaikoBeatmap/TaikoDrumRoll.cpp
        src/Beatmap/TaikoBeatmap/TaikoDrumRoll.h
        src/Platform/Psp/Core.cpp
        src/Platform/Psp/Core.h
        src/Platform/Psp/Graphics/Texture.cpp
        src/Platform/Psp/Graphics/Texture.h
        src/Platform/Psp/Graphics/Font.cpp
        src/Platform/Psp/Graphics/Font.h
        src/Platform/Psp/Graphics/Swizzle.cpp
        src/Platform/Psp/Graphics/Swizzle.h
        src/Platform/Psp/Graphics/2d.cpp
        src/Platform/Psp/Graphics/2d.h
        src/Platform/Psp/Vector2.h
        src/Platform/Psp/Rectangle.h
        src/Platform/Psp/Graphics/Vertex.h
        src/Input/InputHandling_Psp.cpp
        src/Input/InputHandling.h
        src/BeatmapPlayer/BeatmapPlayer.cpp
        src/BeatmapPlayer/BeatmapPlayer.h
        src/BeatmapPlayer/BeatmapPlayerPsp.cpp
        src/BeatmapPlayer/BeatmapPlayerPsp.h
        src/Ruleset/Ruleset.cpp
        src/Ruleset/Ruleset.h
        src/Ruleset/TaikoRuleset/TaikoRuleset.cpp
        src/Ruleset/TaikoRuleset/TaikoRuleset.h
        src/Ruleset/TaikoRuleset/TaikoRulesetPsp.cpp
        src/Ruleset/TaikoRuleset/TaikoRulesetPsp.h
        src/Platform/Psp/Audio/AudioStream.cpp
        src/Platform/Psp/Audio/AudioStream.h
        src/Platform/Psp/Audio/AudioStreamMP3.cpp
        src/Platform/Psp/Audio/AudioStreamMP3.h
        src/Platform/Psp/Audio/AudioThread.cpp
        src/Platform/Psp/Audio/AudioThread.h
        src/Platform/Psp/Audio/AudioStreamOGG.cpp
        src/Platform/Psp/Audio/AudioStreamOGG.h
        src/Platform/Psp/Audio/resample.cpp
        src/Platform/Psp/Audio/resample.h
        src/SceneManagement/SceneManager.cpp
        src/SceneManagement/SceneManager.h
        src/SceneManagement/Scene.cpp
        src/SceneManagement/Scene.h
        src/MapIndex/MapIndexFile.cpp
        src/MapIndex/MapIndexFile.h
        src/Registry/FileFormat/FileFormatEntry.h
        src/Registry/FileFormat/FileFormatRegistry.h
        src/Registry/FileFormat/FileFormatRegistry.cpp
        src/Registry/FileFormat/FileFormatRegistry_psp.cpp
        src/Beatmap/OsuBeatmap/OsuBeatmapParsing.cpp
        src/BeatmapPlayer/BeatmapLoader.cpp
        src/BeatmapPlayer/BeatmapLoader.h
)

# TODO: remove all the unnecessary stuff
target_link_libraries(${PROJECT_NAME} PRIVATE
        pspdebug
        pspctrl
        pspgu
        pspgum
        pspge
        pspdisplay
        pspaudio
        pspaudiolib
)

target_include_directories(${PROJECT_NAME} PRIVATE
    external/minimp3
    external/stb_vorbis
)

# EBOOT.PBP
create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
        VERSION 01.00
)
