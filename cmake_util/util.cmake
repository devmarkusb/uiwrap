cmake_minimum_required(VERSION 3.14)

# mb.util exposes target `mb.util` / `mb::util` (INTERFACE). When the parent project already added it, skip FetchContent.
if(TARGET mb.util)
    return()
endif()

include(FetchContent)

FetchContent_Declare(
    mb_ul_fetch
    GIT_REPOSITORY https://github.com/devmarkusb/util
    GIT_TAG main
    GIT_SHALLOW ON
    GIT_SUBMODULES_RECURSE TRUE
)

FetchContent_MakeAvailable(mb_ul_fetch)
