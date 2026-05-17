cmake_minimum_required(VERSION 3.30...4.3)

# Public cache: MB_UIWRAP_BUILD_NR (legacy -DBUILD_NR= is still honored when MB_UIWRAP_BUILD_NR is unset).
if(DEFINED BUILD_NR AND NOT DEFINED MB_UIWRAP_BUILD_NR)
    set(MB_UIWRAP_BUILD_NR
        "${BUILD_NR}"
        CACHE STRING
        "Build number segment for mb.uiwrap version (0.1.<nr>.0)."
        FORCE
    )
endif()
if(NOT DEFINED MB_UIWRAP_BUILD_NR)
    set(MB_UIWRAP_BUILD_NR
        "0"
        CACHE STRING
        "Build number segment for mb.uiwrap version (0.1.<nr>.0)."
    )
endif()

# Standalone tree: apply FetchContent lockfile before project() when CMakePresets do not inject it via
# CMAKE_PROJECT_TOP_LEVEL_INCLUDES (see devmarkusb/cpp-lib-template).
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(_mb_uiwrap_skip_lockfile 0)
    if(
        DEFINED CMAKE_PROJECT_TOP_LEVEL_INCLUDES
        AND CMAKE_PROJECT_TOP_LEVEL_INCLUDES
    )
        foreach(_mb_uiwrap_inc IN LISTS CMAKE_PROJECT_TOP_LEVEL_INCLUDES)
            if(_mb_uiwrap_inc MATCHES "fetch-content-from-lockfile")
                set(_mb_uiwrap_skip_lockfile 1)
                break()
            endif()
        endforeach()
    endif()
    if(
        NOT _mb_uiwrap_skip_lockfile
        AND EXISTS
            "${CMAKE_CURRENT_SOURCE_DIR}/devenv/cmake/fetch-content-from-lockfile.cmake"
    )
        include(
            "${CMAKE_CURRENT_SOURCE_DIR}/devenv/cmake/fetch-content-from-lockfile.cmake"
        )
    endif()
endif()

project(
    mb.uiwrap
    DESCRIPTION
        "UI-adjacent abstraction (filesystem, message boxes, settings, Qt helpers)"
    LANGUAGES CXX
    VERSION 0.1.${MB_UIWRAP_BUILD_NR}.0
)

# mb.util on main requires at least C++20; uiwrap standardizes on C++23 (MSVC <stacktrace>, presets).
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# [CMAKE.SKIP_TESTS] — marker for template-style tooling (see devmarkusb/cpp-lib-template).
option(
    MB_UIWRAP_BUILD_TESTS
    "Enable building uiwrap unit tests (Google Test). Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

if(PROJECT_IS_TOP_LEVEL)
    list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/devenv/cmake")
    if(MB_UIWRAP_BUILD_TESTS)
        include(mb-devenv-ctest-root)
    endif()
    include(mb-devenv-defaults)
    include(mb-devenv-install-library-config)
endif()

# From fetched https://github.com/devmarkusb/pre-commit (see fetchcontent-lockfile.json).
mb_pre_commit_setup_project()

if(NOT PROJECT_IS_TOP_LEVEL AND MB_UIWRAP_BUILD_TESTS)
    if(DEFINED mb_ul_fetch_SOURCE_DIR)
        set(_mb_uiwrap_ctest_root
            "${mb_ul_fetch_SOURCE_DIR}/devenv/cmake/mb-devenv-ctest-root.cmake"
        )
        if(EXISTS "${_mb_uiwrap_ctest_root}")
            list(
                PREPEND CMAKE_MODULE_PATH
                "${mb_ul_fetch_SOURCE_DIR}/devenv/cmake"
            )
            include(mb-devenv-ctest-root)
        endif()
    endif()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake_util/util.cmake)

mb_ul_include(buildnr.cmake)
set(target uiwrap)
message(
    STATUS
    "${PROJECT_NAME} (library target: ${target}) version: "
    ${PROJECT_VERSION}
)

if(MSVC)
    # not good, I only intended to suppress this for Qt headers but didn't find a way
    add_definitions(-wd4127)
endif()

# Public cache: MB_UIWRAP_USE_IMPLEMENTATION (legacy -Duiwrap_USE_IMPLEMENTATION= is still honored on first configure).
if(
    DEFINED uiwrap_USE_IMPLEMENTATION
    AND NOT DEFINED MB_UIWRAP_USE_IMPLEMENTATION
)
    set(MB_UIWRAP_USE_IMPLEMENTATION
        "${uiwrap_USE_IMPLEMENTATION}"
        CACHE STRING
        "UI backend: qt, wx (stub), or own (CLI / no toolkit)."
        FORCE
    )
endif()
if(NOT DEFINED MB_UIWRAP_USE_IMPLEMENTATION)
    set(MB_UIWRAP_USE_IMPLEMENTATION
        "own"
        CACHE STRING
        "UI backend: qt, wx (stub), or own (CLI / no toolkit)."
    )
endif()

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "own")
    set(MB_UIWRAP_BOOST_MIN_VERSION
        "1.67"
        CACHE STRING
        "Minimum Boost for the \"own\" backend (header-only property_tree)."
    )
    find_package(Boost ${MB_UIWRAP_BOOST_MIN_VERSION} CONFIG REQUIRED)
endif()

option(
    MB_UIWRAP_DISABLE_NAMESPACE_ALIAS
    "ON: base namespace is mb::uiw; OFF (default): ul:: is an alias for mb::uiw::"
    OFF
)
if(DEFINED UIW_DISABLE_NAMESPACE_ALIAS)
    set(MB_UIWRAP_DISABLE_NAMESPACE_ALIAS
        "${UIW_DISABLE_NAMESPACE_ALIAS}"
        CACHE BOOL
        "ON: base namespace is mb::uiw; OFF (default): ul:: is an alias for mb::uiw::"
        FORCE
    )
endif()
set(UIW_DISABLE_NAMESPACE_ALIAS "${MB_UIWRAP_DISABLE_NAMESPACE_ALIAS}")
configure_file(
    ${PROJECT_SOURCE_DIR}/include/uiwrap/config_gen.h.in
    ${PROJECT_SOURCE_DIR}/include/uiwrap/config_gen.h
)

file(GLOB_RECURSE target_INCS "include/*.h")
file(GLOB_RECURSE target_SRC_INCS "src/*.h")

set(target_COMMON_SOURCES
    src/filesys/filesys.cpp
    src/linklib/linklib.cpp
    src/messagebox/messagebox.cpp
    src/programsettings/programsettings.cpp
)

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    mb_ul_include(qt.cmake)
    # Widgets contains (a.o.t.) Core and Gui; and is needed for QMessageBox
    # Qml is needed e.g. for QQmlApplicationEngine (Quick would do also - could not find detailed infos yet, which to choose)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Widgets)
    qt_standard_project_setup(REQUIRES 6.8)

    set(target_IMPL_SOURCES
        include/uiwrap/GUI/impl_Qt/qmlextlib.cpp
        src/filesys/impl_Qt/filesys_Qt.cpp
        src/messagebox/impl_Qt/MessageBox_Qt.cpp
        src/programsettings/impl_Qt/ProgSettings_Qt.cpp
    )

elseif(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "wx")
    set(target_IMPL_SOURCES "")

else()
    set(target_IMPL_SOURCES
        src/filesys/impl_/filesys_.cpp
        src/messagebox/impl_/messagebox_.cpp
        src/programsettings/impl_/ProgSettings_.cpp
    )
endif()

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    qt_add_library(${target} SHARED
        ${target_COMMON_SOURCES}
        ${target_IMPL_SOURCES}
        ${target_INCS}
        ${target_SRC_INCS}
    )
else()
    add_library(
        ${target}
        SHARED
        ${target_COMMON_SOURCES}
        ${target_IMPL_SOURCES}
        ${target_INCS}
        ${target_SRC_INCS}
    )
endif()

mb_devenv_set_target_lib_export_def(${target} UIWRAP_LIBRARY)

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    set(uiwrap_impl_choice_number 1)
elseif(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "wx")
    set(uiwrap_impl_choice_number 2)
else()
    set(uiwrap_impl_choice_number 0)
endif()

configure_file(
    ${target}_build_config.h.in
    ${CMAKE_BINARY_DIR}/${target}_build_config.h
)
target_include_directories(${target} PUBLIC ${CMAKE_BINARY_DIR})

target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PRIVATE src)
if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "own")
    if(TARGET Boost::headers)
        target_link_libraries(${target} PUBLIC Boost::headers)
    else()
        target_include_directories(
            ${target}
            SYSTEM
            PUBLIC ${Boost_INCLUDE_DIRS}
        )
    endif()
    target_compile_definitions(
        ${target}
        PRIVATE BOOST_ALL_NO_LIB BOOST_FILESYSTEM_NO_DEPRECATED
    )
endif()

target_link_libraries(${target} PUBLIC mb::util)

if(MB_DEVENV_LINUX)
    target_link_libraries(${target} PUBLIC dl)
endif()

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    target_link_libraries(${target} PUBLIC Qt::Core Qt::Gui Qt::Qml Qt::Widgets)
endif()

mb_devenv_set_target_defaults(${target})

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL
    "${target}: include directories"
    FORCE
)

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

set(targetTest_SOURCES src/filesys/filesys.test.cpp)

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    set(targetTest_IMPL_SOURCES
        src/fundamental_types/impl_Qt/convert_Qt.test.cpp
        src/GUI/impl_Qt/cppmodel2qml.test.cpp
        src/GUI/impl_Qt/qmlextlib.test.cpp
        src/string/impl_Qt/urlstring_util_Qt.test.cpp
    )
elseif(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "wx")
    set(targetTest_IMPL_SOURCES "")
else()
    set(targetTest_IMPL_SOURCES src/messagebox/impl_/messagebox_.test.cpp)
endif()

add_executable(
    ${targetTest}
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
    message(
        FATAL_ERROR
        "uiwrap tests: expected GTest::gtest_main or gtest_main target after GTest setup."
    )
endif()

target_link_libraries(${targetTest} PRIVATE ${_mb_uiwrap_gtest_main} ${target})

mb_devenv_set_target_defaults(${targetTest})

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    mb_ul_include(qt-deploy.cmake)
    mb_ul_qt_deploy(${targetTest} "")
endif()

add_test(NAME uiwrap_unit_tests COMMAND $<TARGET_FILE:${targetTest}>)
