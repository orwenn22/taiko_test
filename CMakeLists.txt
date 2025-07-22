cmake_minimum_required(VERSION 3.31)
project(taiko_test)

# to build for psp run with -DCMAKE_TOOLCHAIN_FILE="${PSPDEV}/psp/share/pspdev.cmake"

if (DEFINED PSP)
    message(STATUS "PSP defined, building for psp")
    include(PlatformPsp.cmake)
else()
    message(STATUS "building for raylib")
    include(PlatformRaylib.cmake)
endif()