# Some notes for the occasional CMake user.
#
# 1 Debug or Release or...
# start cmake with arg either
# -DCMAKE_BUILD_TYPE=Debug
# or
# -DCMAKE_BUILD_TYPE=Release
# Other options are: MinSizeRel, RelWithDebInfo
#
# 2 Dynamic vs Static C/C++ runtime lib
# There is no support for static runtime libs! You could achieve this
# for MSVC, but this is not recommended. You or other will end up in
# conflicts soon.

cmake_minimum_required(VERSION 3.4.1)

include(buildnr.cmake)
project(uiwrap VERSION 0.1.0.${BUILD_NR})
message(STATUS "project version: " ${PROJECT_VERSION})
set(TargetUIWrap "uiwrap")

include(defaults.cmake)

set(uiwrap_USE_IMPLEMENTATION "own" CACHE STRING "Choose the desired UI implementation out of: qt, wx,... or own (the default)")

file(GLOB_RECURSE TargetUIWrap_INCS "include/*.h")
file(GLOB_RECURSE TargetUIWrap_SRC_INCS "src/*.h")

set(TargetUIWrap_COMMON_SOURCES
    src/filesys/filesys.cpp
    src/linklib/linklib.cpp
    src/messagebox/messagebox.cpp
    src/programsettings/programsettings.cpp
    )

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(TargetUIWrap_IMPL_SOURCES
        src/filesys/impl_Qt/FileSys_Qt.cpp
        src/messagebox/impl_Qt/MessageBox_Qt.cpp
        src/programsettings/impl_Qt/ProgSettings_Qt.cpp
        )

    if (NOT Qt5Widgets_INCLUDE_DIRS)
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        set(CMAKE_AUTOMOC ON)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(QT_COMPILER_SUBDIR "mingw53_32")
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            set(QT_COMPILER_SUBDIR "msvc2013")
        endif ()
        list(APPEND CMAKE_PREFIX_PATH "$ENV{dev_qt_base}/5.7/${QT_COMPILER_SUBDIR}")
        find_package(Qt5 COMPONENTS Widgets Qml) # Widgets contains a.o.t. Core and Gui
    endif ()

elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    #    set(TargetUIWrap_IMPL_SOURCES
    #        )
else ()
    set(TargetUIWrap_IMPL_SOURCES
        src/filesys/impl_/FileSys_.cpp
        )
endif ()

add_library(${TargetUIWrap} SHARED
    ${TargetUIWrap_COMMON_SOURCES}
    ${TargetUIWrap_IMPL_SOURCES}
    ${TargetUIWrap_INCS}
    ${TargetUIWrap_SRC_INCS}
    )

if (WIN32)
    set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UIW_LINKLIB_IMPL_CHOICE_QT)
elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UIW_LINKLIB_IMPL_CHOICE_WX)
else ()
    set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UIW_LINKLIB_IMPL_CHOICE_OWN)
endif ()

# dll export define
set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UIWRAP_LIBRARY)

set_target_properties(${TargetUIWrap} PROPERTIES DEBUG_POSTFIX "d")

if (NOT Boost_INCLUDE_DIRS)
    set(BOOST_ROOT $ENV{dev_sdk_paths})
    set(Boost_USE_MULTITHREADED     ON)
    set(Boost_USE_STATIC_LIBS       OFF)
    set(Boost_USE_STATIC_RUNTIME    OFF)
    find_package(Boost 1.56.0)
endif ()

if (NOT Toolib_INCLUDE_DIRS)
    set(Toolib_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/../Toolib/include)
endif ()
if (NOT Toolib_LIBRARY_DIRS)
    set(Toolib_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/../Toolib/lib)
endif ()

set(Toolib_LIBRARY_NAME optimized Toolib debug Toolibd)
find_library(ToolibPreBuilt ${Toolib_LIBRARY_NAME} HINTS ${Toolib_LIBRARY_DIRS})

configure_file(${TargetUIWrap}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetUIWrap}_build_config.h)
target_include_directories(${TargetUIWrap} PRIVATE ${CMAKE_BINARY_DIR})

target_include_directories(${TargetUIWrap} PRIVATE include)
target_include_directories(${TargetUIWrap} PRIVATE src)
target_include_directories(${TargetUIWrap} SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
target_include_directories(${TargetUIWrap} SYSTEM PUBLIC ${Qt5Widgets_INCLUDE_DIRS})
target_include_directories(${TargetUIWrap} SYSTEM PUBLIC ${Qt5Qml_INCLUDE_DIRS})
endif ()
target_include_directories(${TargetUIWrap} PRIVATE ${Toolib_INCLUDE_DIRS})
target_include_directories(${TargetUIWrap} PRIVATE ../ToolsFL/tooBasicCodeSupport/include)
target_include_directories(${TargetUIWrap} PRIVATE ../ToolsFL/tooBuildEnv/include)
target_include_directories(${TargetUIWrap} PRIVATE ../ToolsFL/tooString/include)

if (ToolibPreBuilt)
    target_link_libraries(${TargetUIWrap} PUBLIC ${ToolibPreBuilt})
else ()
    target_link_libraries(${TargetUIWrap} PUBLIC ${Toolib})
endif ()

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
target_link_libraries(${TargetUIWrap} PUBLIC Qt5::Widgets)
target_link_libraries(${TargetUIWrap} PUBLIC Qt5::Qml)
endif ()


################################################################################################

set(TargetUIWrapTest "UIWrapTest")

file(GLOB_RECURSE TargetUIWrapTest_SRC_INCS "test/*.h")

set(TargetUIWrapTest_SOURCES
    test/filesys/filesys.test.cpp
    )

add_executable(${TargetUIWrapTest}
    ${TargetUIWrapTest_SOURCES}
    ${TargetUIWrapTest_SRC_INCS}
    )

if (WIN32)
    set_property(TARGET ${TargetUIWrapTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetUIWrapTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT $ENV{dev_sdk_paths})
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${TargetUIWrapTest} PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(${TargetUIWrapTest} PRIVATE include)
target_include_directories(${TargetUIWrapTest} PRIVATE ${Toolib_INCLUDE_DIRS})
target_include_directories(${TargetUIWrapTest} PRIVATE ../ToolsFL/tooBasicCodeSupport/include)
target_include_directories(${TargetUIWrapTest} PRIVATE ../ToolsFL/tooBuildEnv/include)
target_include_directories(${TargetUIWrapTest} PRIVATE ../ToolsFL/tooString/include)

target_link_libraries(${TargetUIWrapTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TargetUIWrapTest} PRIVATE ${TargetUIWrap})


add_custom_command(
    TARGET ${TargetUIWrapTest} POST_BUILD
    COMMAND ${TargetUIWrapTest}
    )

