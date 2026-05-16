cmake_minimum_required(VERSION 3.14)

# mb.util exposes target `mb.util` / `mb::util` (INTERFACE). When the parent project already added it, skip FetchContent.
if(TARGET mb.util)
    return()
endif()

include(FetchContent)

# Parent apps (e.g. WealthPlanner) fetch mb.util for headers/targets only, not install.
set(MB_UTIL_INSTALL_CONFIG_FILE_PACKAGE OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    mb_ul_fetch
    GIT_REPOSITORY https://github.com/devmarkusb/util
    GIT_TAG main
    GIT_SHALLOW ON
    GIT_SUBMODULES_RECURSE TRUE
)

FetchContent_MakeAvailable(mb_ul_fetch)
