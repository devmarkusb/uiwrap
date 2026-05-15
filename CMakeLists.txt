cmake_minimum_required(VERSION 3.30...4.3)

# Older uiwrap revisions set CMAKE_PROJECT_TOP_LEVEL_INCLUDES to a removed hook file; drop stale cache entries.
if(DEFINED CACHE{CMAKE_PROJECT_TOP_LEVEL_INCLUDES})
    if(
        CMAKE_PROJECT_TOP_LEVEL_INCLUDES MATCHES "uiwrap_fetch_mb_pre_commit"
        AND NOT EXISTS "${CMAKE_PROJECT_TOP_LEVEL_INCLUDES}"
    )
        unset(CMAKE_PROJECT_TOP_LEVEL_INCLUDES CACHE)
    endif()
endif()

# mb.util calls mb_pre_commit_setup() (real impl from devmarkusb/pre-commit when using util's CMakePresets).
# Standalone uiwrap does not need that machinery; provide a no-op if the parent did not define the macro.
if(NOT COMMAND mb_pre_commit_setup)
    macro(mb_pre_commit_setup)
    endmacro()
endif()

# Default build number (overridden by mb_ul_include(buildnr.cmake) after mb.util is available).
set(BUILD_NR "0" CACHE STRING "Should be set by your buildserver")

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

include(${CMAKE_CURRENT_LIST_DIR}/cmake_util/util.cmake)

if(MB_UIWRAP_BUILD_TESTS)
    if(DEFINED mb_ul_fetch_SOURCE_DIR)
        set(_mb_uiwrap_ctest_root "${mb_ul_fetch_SOURCE_DIR}/devenv/cmake/mb-devenv-ctest-root.cmake")
        if(EXISTS "${_mb_uiwrap_ctest_root}")
            list(PREPEND CMAKE_MODULE_PATH "${mb_ul_fetch_SOURCE_DIR}/devenv/cmake")
            include(mb-devenv-ctest-root)
        endif()
    endif()
endif()

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

if(NOT TARGET gtest_main)
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

target_link_libraries(${targetTest} PRIVATE gtest_main ${target})

mb_devenv_set_target_defaults(${targetTest})

if(uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    mb_ul_include(qt-deploy.cmake)
    mb_ul_qt_deploy(${targetTest} "")
endif()

add_test(NAME uiwrap_unit_tests COMMAND $<TARGET_FILE:${targetTest}>)
