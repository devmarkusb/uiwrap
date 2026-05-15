cmake_minimum_required(VERSION 3.14)

# When uiwrap is embedded, the parent normally provides `mb-util` (INTERFACE target from legacy util).
# Do not use `ulBuildEnv` as the guard: other subtrees can create it without exposing `mb-util`,
# and the new util rewrite on `main` does not create either of those the same way.
if (TARGET mb-util)
    return ()
endif ()

include(FetchContent)

# `main` at devmarkusb/util is the mb.util rewrite (CMake 3.30+, `mb::util`, different layout) and is
# not compatible with this uiwrap tree, which still expects legacy `ul/` + target `mb-util`.
# Override with -DMB_UTIL_FETCH_TAG=... when intentionally moving to a newer compatible revision.
set(MB_UTIL_FETCH_TAG "b38a367953f5db121498886074cb5f2acae8e14e" CACHE STRING
    "Git ref (commit, branch, or tag) for devmarkusb/util when FetchContent-populated as mb-util")

FetchContent_Declare(
    mb-util
    GIT_REPOSITORY https://github.com/devmarkusb/util
    GIT_TAG ${MB_UTIL_FETCH_TAG}
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(mb-util)
