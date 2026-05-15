cmake_minimum_required(VERSION 3.30...4.3)

# Default build number (overridden by mb_ul_include(buildnr.cmake) after mb.util is available).
set(BUILD_NR "0" CACHE STRING "Should be set by your buildserver")

# Standalone tree: apply FetchContent lockfile before project() when CMakePresets do not inject it via
# CMAKE_PROJECT_TOP_LEVEL_INCLUDES (see devmarkusb/cpp-lib-template).
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(_mb_uiwrap_skip_lockfile 0)
    if(DEFINED CMAKE_PROJECT_TOP_LEVEL_INCLUDES AND CMAKE_PROJECT_TOP_LEVEL_INCLUDES)
        foreach(_mb_uiwrap_inc IN LISTS CMAKE_PROJECT_TOP_LEVEL_INCLUDES)
            if(_mb_uiwrap_inc MATCHES "fetch-content-from-lockfile")
                set(_mb_uiwrap_skip_lockfile 1)
                break()
            endif()
        endforeach()
    endif()
    if(
        NOT _mb_uiwrap_skip_lockfile
        AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/devenv/cmake/fetch-content-from-lockfile.cmake"
    )
        include("${CMAKE_CURRENT_SOURCE_DIR}/devenv/cmake/fetch-content-from-lockfile.cmake")
    endif()
endif()

project(
    mb.uiwrap
    DESCRIPTION "UI-adjacent abstraction (filesystem, message boxes, settings, Qt helpers)"
    LANGUAGES CXX
    VERSION 0.1.${BUILD_NR}.0
)

# mb.util headers on main require C++20 (e.g. std::integral, concepts).
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# [CMAKE.SKIP_TESTS] — marker for template-style tooling (see devmarkusb/cpp-lib-template).
option(
    MB_UIWRAP_BUILD_TESTS
    "Enable building uiwrap unit tests (Google Test). Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

if(PROJECT_IS_TOP_LEVEL)
    # cpp-lib-template uses add_subdirectory(devenv), which configures the devenv submodule and runs
    # mb_pre_commit_setup there first. That nested configure uses CUSTOM hook install and fails when
    # this repo sets core.hooksPath (common with a checked-in .githooks directory). We only need
    # devenv/cmake on the module path for toolchains and mb-devenv-* includes; the outer
    # mb_pre_commit_setup() below matches the template's project()-level call.
    list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/devenv/cmake")
    if(MB_UIWRAP_BUILD_TESTS)
        include(mb-devenv-ctest-root)
    endif()
    include(mb-devenv-defaults)
    include(mb-devenv-install-library-config)
    execute_process(
        COMMAND git -C "${CMAKE_SOURCE_DIR}" config --get core.hooksPath
        OUTPUT_VARIABLE _mb_uiwrap_core_hooks_path
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE _mb_uiwrap_core_hooks_path_rc
        ERROR_QUIET
    )
    if(_mb_uiwrap_core_hooks_path_rc EQUAL 0 AND NOT _mb_uiwrap_core_hooks_path STREQUAL "")
        message(
            STATUS
            "mb_pre_commit_setup skipped: git core.hooksPath='${_mb_uiwrap_core_hooks_path}'. "
            "Unset with: git config --unset-all core.hooksPath (then reconfigure) to match cpp-lib-template hook install."
        )
    else()
        mb_pre_commit_setup()
    endif()
else()
    if(MB_UIWRAP_BUILD_TESTS)
        if(DEFINED mb_ul_fetch_SOURCE_DIR)
            set(_mb_uiwrap_ctest_root "${mb_ul_fetch_SOURCE_DIR}/devenv/cmake/mb-devenv-ctest-root.cmake")
            if(EXISTS "${_mb_uiwrap_ctest_root}")
                list(PREPEND CMAKE_MODULE_PATH "${mb_ul_fetch_SOURCE_DIR}/devenv/cmake")
                include(mb-devenv-ctest-root)
            endif()
        endif()
    endif()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake_util/util.cmake)

mb_ul_include(buildnr.cmake)
set(target uiwrap)
message(STATUS "${PROJECT_NAME} (library target: ${target}) version: " ${PROJECT_VERSION})

if(MSVC)
    # not good, I only intended to suppress this for Qt headers but didn't find a way
    add_definitions(-wd4127)
endif()

set(uiwrap_USE_IMPLEMENTATION "own" CACHE STRING "Choose the desired UI implementation out of: qt, wx, ... or \
own (the default, a CLI)")

if(uiwrap_USE_IMPLEMENTATION STREQUAL "own")
    mb_ul_include(boost.cmake)
    mb_ul_find_boost(${MB_UL_USE_BOOST_ver1} ${MB_UL_USE_BOOST_ver2} ${MB_UL_USE_BOOST_ver3} "")
endif()

option(UIW_DISABLE_NAMESPACE_ALIAS "Set to ON if you want the base namespace to be mb::uiw instead of ul" OFF)
configure_file(
    ${PROJECT_SOURCE_DIR}/include/uiwrap/config_gen.h.in
    ${PROJECT_SOURCE_DIR}/include/uiwrap/config_gen.h
    @ONLY)

file(GLOB_RECURSE target_INCS "include/*.h")
file(GLOB_RECURSE target_SRC_INCS "src/*.h")

set(target_COMMON_SOURCES
    src/filesys/filesys.cpp
    src/linklib/linklib.cpp
    src/messagebox/messagebox.cpp
    src/programsettings/programsettings.cpp
)

if(uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    mb_ul_include(qt.cmake)
    # Widgets contains (a.o.t.) Core and Gui; and is needed fo QMessageBox
    # Qml is needed e.g. for QQmlApplicationEngine (Quick would do also - could not find detailed infos yet, which to choose)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Widgets)
    qt_standard_project_setup(REQUIRES 6.8)

    set(target_IMPL_SOURCES
        include/uiwrap/GUI/impl_Qt/qmlextlib.cpp
        src/filesys/impl_Qt/filesys_Qt.cpp
        src/messagebox/impl_Qt/MessageBox_Qt.cpp
        src/programsettings/impl_Qt/ProgSettings_Qt.cpp
    )

elseif(uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set(target_IMPL_SOURCES "")

else()
    set(target_IMPL_SOURCES
        src/filesys/impl_/filesys_.cpp
        src/messagebox/impl_/messagebox_.cpp
    )

endif()

if(uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    qt_add_library(${target} SHARED
        ${target_COMMON_SOURCES}
        ${target_IMPL_SOURCES}
        ${target_INCS}
        ${target_SRC_INCS}
    )
else()
    add_library(${target} SHARED
        ${target_COMMON_SOURCES}
        ${target_IMPL_SOURCES}
        ${target_INCS}
        ${target_SRC_INCS}
    )
endif()

mb_devenv_set_target_lib_export_def(${target} UIWRAP_LIBRARY)

if(uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(uiwrap_impl_choice_number 1)
elseif(uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set(uiwrap_impl_choice_number 2)
else()
    set(uiwrap_impl_choice_number 0)
endif()

configure_file(${target}_build_config.h.in ${CMAKE_BINARY_DIR}/${target}_build_config.h)
target_include_directories(${target} PUBLIC ${CMAKE_BINARY_DIR})

target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PRIVATE src)
if(uiwrap_USE_IMPLEMENTATION STREQUAL "own")
    target_include_directories(${target} SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
endif()

target_link_libraries(${target} PUBLIC mb::util)

if(MB_DEVENV_LINUX)
    target_link_libraries(${target} PUBLIC dl)
endif()

if(uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_link_libraries(${target} PUBLIC Qt::Core Qt::Gui Qt::Qml Qt::Widgets)
endif()

mb_devenv_set_target_defaults(${target})

set(${target}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${target}: include directories" FORCE)

if(NOT MB_UIWRAP_BUILD_TESTS)
    return()
endif()

if(NOT TARGET GTest::gtest_main)
    find_package(GTest QUIET)
endif()
if(NOT TARGET GTest::gtest_main)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

set(targetTest uiwrapTest)

set(targetTest_SOURCES
    src/filesys/filesys.test.cpp
)

if(uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(targetTest_IMPL_SOURCES
        src/fundamental_types/impl_Qt/convert_Qt.test.cpp
        src/GUI/impl_Qt/cppmodel2qml.test.cpp
        src/GUI/impl_Qt/qmlextlib.test.cpp
        src/string/impl_Qt/urlstring_util_Qt.test.cpp
    )
elseif(uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set(targetTest_IMPL_SOURCES "")
else()
    set(targetTest_IMPL_SOURCES
        src/messagebox/impl_/messagebox_.test.cpp
    )
endif()

add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${targetTest_IMPL_SOURCES}
    ${target_INCS}
)

target_include_directories(${targetTest} PRIVATE ${uiwrap_INCLUDE_DIRS})

if(TARGET GTest::gtest_main)
    set(_mb_uiwrap_gtest_main GTest::gtest_main)
elseif(TARGET gtest_main)
    set(_mb_uiwrap_gtest_main gtest_main)
else()
    message(FATAL_ERROR "uiwrap tests: expected GTest::gtest_main or gtest_main target after GTest setup.")
endif()

target_link_libraries(${targetTest} PRIVATE ${_mb_uiwrap_gtest_main} ${target})

mb_devenv_set_target_defaults(${targetTest})

if(uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    mb_ul_include(qt-deploy.cmake)
    mb_ul_qt_deploy(${targetTest} "")
endif()

add_test(NAME uiwrap_unit_tests COMMAND $<TARGET_FILE:${targetTest}>)
