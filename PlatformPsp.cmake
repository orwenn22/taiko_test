set(CMAKE_CXX_STANDARD 20)

file(COPY res_psp/ DESTINATION res)


add_executable(${PROJECT_NAME} src/Psp/main_Psp.cpp
        src/Base/Utils.h
        src/Base/Utils.cpp
        src/Base/Beatmap/Beatmap.cpp
        src/Base/Beatmap/Beatmap.h
        src/Base/Beatmap/OsuBeatmap/OsuBeatmap.cpp
        src/Base/Beatmap/OsuBeatmap/OsuBeatmap.h
        src/Base/Beatmap/OsuBeatmap/OsuHitObject.cpp
        src/Base/Beatmap/OsuBeatmap/OsuHitObject.h
        src/Base/Beatmap/OsuBeatmap/OsuTimingPoint.cpp
        src/Base/Beatmap/OsuBeatmap/OsuTimingPoint.h
        src/Base/Beatmap/TaikoBeatmap/TaikoHit.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoHit.h
        src/Base/Beatmap/TaikoBeatmap/TaikoEffectPoint.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoEffectPoint.h
        src/Base/Beatmap/TaikoBeatmap/TaikoBeatmap.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoBeatmap.h
        src/Base/Beatmap/TaikoBeatmap/TaikoTimingPoint.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoTimingPoint.h
        src/Base/Beatmap/TaikoBeatmap/TaikoDrumRoll.cpp
        src/Base/Beatmap/TaikoBeatmap/TaikoDrumRoll.h
        src/Psp/Platform/Core.cpp
        src/Psp/Platform/Core.h
        src/Psp/Platform/Graphics/Texture.cpp
        src/Psp/Platform/Graphics/Texture.h
        src/Psp/Platform/Graphics/Font.cpp
        src/Psp/Platform/Graphics/Font.h
        src/Psp/Platform/Graphics/Swizzle.cpp
        src/Psp/Platform/Graphics/Swizzle.h
        src/Psp/Platform/Graphics/2d.cpp
        src/Psp/Platform/Graphics/2d.h
        src/Psp/Platform/Vector2.h
        src/Psp/Platform/Rectangle.h
        src/Psp/Platform/Graphics/Vertex.h
        src/Psp/Input/InputHandling_Psp.cpp
        src/Base/Input/InputHandling.h
        src/Base/BeatmapPlayer/BeatmapPlayer.cpp
        src/Base/BeatmapPlayer/BeatmapPlayer.h
        src/Psp/BeatmapPlayer/BeatmapPlayerPsp.cpp
        src/Psp/BeatmapPlayer/BeatmapPlayerPsp.h
        src/Base/Ruleset/Ruleset.cpp
        src/Base/Ruleset/Ruleset.h
        src/Base/Ruleset/TaikoRuleset/TaikoRuleset.cpp
        src/Base/Ruleset/TaikoRuleset/TaikoRuleset.h
        src/Psp/Ruleset/TaikoRuleset/TaikoRulesetPsp.cpp
        src/Psp/Ruleset/TaikoRuleset/TaikoRulesetPsp.h
        src/Psp/Platform/Audio/AudioStream.cpp
        src/Psp/Platform/Audio/AudioStream.h
        src/Psp/Platform/Audio/AudioStreamMP3.cpp
        src/Psp/Platform/Audio/AudioStreamMP3.h
        src/Psp/Platform/Audio/AudioThread.cpp
        src/Psp/Platform/Audio/AudioThread.h
        src/Psp/Platform/Audio/AudioStreamOGG.cpp
        src/Psp/Platform/Audio/AudioStreamOGG.h
        src/Psp/Platform/Audio/resample.cpp
        src/Psp/Platform/Audio/resample.h
        src/Base/SceneManagement/SceneManager.cpp
        src/Base/SceneManagement/SceneManager.h
        src/Base/SceneManagement/Scene.cpp
        src/Base/SceneManagement/Scene.h
        src/Base/MapIndex/MapIndexFile.cpp
        src/Base/MapIndex/MapIndexFile.h
        src/Base/Registry/FileFormat/FileFormatEntry.h
        src/Base/Registry/FileFormat/FileFormatRegistry.h
        src/Base/Registry/FileFormat/FileFormatRegistry.cpp
        src/Psp/Registry/FileFormat/FileFormatRegistry_psp.cpp
        src/Base/Beatmap/OsuBeatmap/OsuBeatmapParsing.cpp
        src/Base/BeatmapPlayer/BeatmapLoader.cpp
        src/Base/BeatmapPlayer/BeatmapLoader.h
        src/Psp/Platform/Graphics/TextureConvert.cpp
        src/Psp/Platform/Graphics/TextureConvert.h
        src/Psp/Platform/FrameTimesInfo.cpp
        src/Psp/Platform/FrameTimesInfo.h
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
