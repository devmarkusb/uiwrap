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

include(${TOO_CMAKE_INC_BUILDNR})
project(uiwrap VERSION 0.1.${BUILD_NR}.0)
message(STATUS "project version: " ${PROJECT_VERSION})
set(TargetUIWrap ${PROJECT_NAME})

include(${TOO_CMAKE_INC_DEFAULTS})
if (MSVC)
    # not good, I only intended to suppress this for Qt headers but didn't find a way
    add_definitions(-wd4127)
endif ()

set(uiwrap_USE_IMPLEMENTATION "own" CACHE STRING "Choose the desired UI implementation out of: qt, wx, ... or own (the default)")

include(${TOO_CMAKE_INC_BOOST})
find_package(Boost 1.63.0)

# we use boost anyway
set(Toolib_USE_NO_DEPENDENCIES OFF CACHE BOOL "")

file(GLOB_RECURSE TargetUIWrap_INCS "include/*.h")
file(GLOB_RECURSE TargetUIWrap_SRC_INCS "src/*.h")

set(TargetUIWrap_COMMON_SOURCES
    src/filesys/filesys.cpp
    src/linklib/linklib.cpp
    src/messagebox/messagebox.cpp
    src/programsettings/programsettings.cpp
    )

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    include (${TOO_CMAKE_INC_QT})
    # Widgets contains (a.o.t.) Core and Gui; and is needed fo QMessageBox
    # Qml is needed e.g. for QQmlApplicationEngine (Quick would do also - could not find detailed infos yet, which to choose)
    find_package(Qt5Widgets)
    find_package(Qt5Qml)

    set(TargetUIWrap_IMPL_SOURCES
        include/uiwrap/GUI/impl_Qt/qmlextlib.cpp
        src/filesys/impl_Qt/FileSys_Qt.cpp
        src/messagebox/impl_Qt/messagebox_qt.cpp
        src/programsettings/impl_Qt/ProgSettings_Qt.cpp
        )

elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set(TargetUIWrap_IMPL_SOURCES ""
        )

else ()
    set(TargetUIWrap_IMPL_SOURCES
        src/filesys/impl_/FileSys_.cpp
        src/messagebox/impl_/messagebox_.cpp
        )

endif ()

add_library(${TargetUIWrap} SHARED
    ${TargetUIWrap_COMMON_SOURCES}
    ${TargetUIWrap_IMPL_SOURCES}
    ${TargetUIWrap_INCS}
    ${TargetUIWrap_SRC_INCS}
    )

set_target_properties(${TargetUIWrap} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (WIN32)
    set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

# dll export define
set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UIWRAP_LIBRARY)

set_target_properties(${TargetUIWrap} PROPERTIES DEBUG_POSTFIX "d")

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(uiwrap_impl_choice_number 1)
elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set(uiwrap_impl_choice_number 2)
else ()
    set(uiwrap_impl_choice_number 0)
endif ()

configure_file(${TargetUIWrap}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetUIWrap}_build_config.h)
target_include_directories(${TargetUIWrap} PUBLIC ${CMAKE_BINARY_DIR})

target_include_directories(${TargetUIWrap} PUBLIC include)
target_include_directories(${TargetUIWrap} PRIVATE src)
target_include_directories(${TargetUIWrap} SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_include_directories(${TargetUIWrap} SYSTEM PUBLIC ${Qt5Qml_INCLUDE_DIRS})
    target_include_directories(${TargetUIWrap} SYSTEM PUBLIC ${Qt5Widgets_INCLUDE_DIRS})
endif ()
target_include_directories(${TargetUIWrap} PUBLIC ${Toolib_INCLUDE_DIRS})

target_link_libraries(${TargetUIWrap} PUBLIC Toolib)

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_link_libraries(${TargetUIWrap} PUBLIC Qt5::Qml)
    target_link_libraries(${TargetUIWrap} PUBLIC Qt5::Widgets)
endif ()

set(${TargetUIWrap}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${TargetUIWrap}: include directories" FORCE)


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(TargetUIWrapTest ${PROJECT_NAME}Test)

set(TargetUIWrapTest_SOURCES
    include/uiwrap/filesys/filesys.test.cpp
    )

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(TargetUIWrapTest_IMPL_SOURCES
        include/uiwrap/fundamental_types/impl_Qt/convert_Qt.test.cpp
        include/uiwrap/GUI/impl_Qt/cppmodel2qml.test.cpp
        include/uiwrap/GUI/impl_Qt/qmlextlib.test.cpp
        include/uiwrap/string/impl_Qt/UrlString_util_Qt.test.cpp
    )
elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set(TargetUIWrapTest_IMPL_SOURCES "")
else ()
    set(TargetUIWrapTest_IMPL_SOURCES
        src/messagebox/impl_/messagebox_.test.cpp
        )
endif ()

add_executable(${TargetUIWrapTest}
    ${TargetUIWrapTest_SOURCES}
    ${TargetUIWrapTest_IMPL_SOURCES}
    ${TargetUIWrap_INCS}
    )

if (WIN32)
    set_property(TARGET ${TargetUIWrapTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetUIWrapTest} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${TargetUIWrapTest} PRIVATE ${uiwrap_INCLUDE_DIRS})

target_link_libraries(${TargetUIWrapTest} PRIVATE gtest_main ${TargetUIWrap})

set(TOO_TEMP_TARGET ${TargetUIWrapTest})
set(TOO_TEMP_QML_DIR)
include(${TOO_CMAKE_INC_QT_DEPLOY_TOO_TEMP_TARGET})

set(do_auto_test    ON)

if (do_auto_test)
    add_custom_command(
        TARGET ${TargetUIWrapTest} POST_BUILD
        COMMAND ${TargetUIWrapTest}
        )
endif ()
