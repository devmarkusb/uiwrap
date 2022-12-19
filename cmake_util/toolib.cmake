cmake_minimum_required(VERSION 3.14)

if (TARGET toolib)
    return ()
endif ()

include(FetchContent)

FetchContent_Declare(mb-toolib
        GIT_REPOSITORY "/home/markus/projects/git/libs/toolib"
        GIT_TAG origin/HEAD
        GIT_SHALLOW ON
        )

FetchContent_MakeAvailable(mb-toolib)
