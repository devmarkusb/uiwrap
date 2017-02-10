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
set(TargetUIWrap ${PROJECT_NAME})

include(defaults.cmake)

set(uiwrap_USE_IMPLEMENTATION "own" CACHE STRING "Choose the desired UI implementation out of: qt, wx,... or own (the default)")

if (NOT Boost_INCLUDE_DIRS)
    set(BOOST_ROOT $ENV{dev_sdk_paths})
    set(Boost_USE_MULTITHREADED     ON)
    set(Boost_USE_STATIC_LIBS       OFF)
    set(Boost_USE_STATIC_RUNTIME    OFF)
    find_package(Boost 1.63.0)
endif ()

file(GLOB_RECURSE TargetUIWrap_INCS "include/*.h")
file(GLOB_RECURSE TargetUIWrap_SRC_INCS "src/*.h")

set(TargetUIWrap_COMMON_SOURCES
    src/filesys/filesys.cpp
    src/linklib/linklib.cpp
    src/messagebox/messagebox.cpp
    src/programsettings/programsettings.cpp
    )

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    if (NOT Qt5Widgets_INCLUDE_DIRS OR NOT Qt5Qml_INCLUDE_DIRS)
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        set(CMAKE_AUTOMOC ON)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(QT_COMPILER_SUBDIR "$ENV{dev_mingw}")
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            set(QT_COMPILER_SUBDIR "$ENV{dev_msvc}")
        endif ()
        list(APPEND CMAKE_PREFIX_PATH "$ENV{dev_qt_base}/$ENV{dev_qt_v}/${QT_COMPILER_SUBDIR}")
        # Widgets contains (a.o.t.) Core and Gui; and is needed fo QMessageBox
        # Qml is needed e.g. for QQmlApplicationEngine (Quick would do also - could not find detailed infos yet, which to choose)
        find_package(Qt5Widgets)
        find_package(Qt5Qml)
    endif ()

    set(TargetUIWrap_IMPL_SOURCES
        include/uiwrap/GUI/impl_Qt/qmlextlib.cpp
        src/filesys/impl_Qt/FileSys_Qt.cpp
        src/messagebox/impl_Qt/MessageBox_Qt.cpp
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

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    add_definitions(-DUIW_LINKLIB_IMPL_CHOICE_QT)
elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    add_definitions(-DUIW_LINKLIB_IMPL_CHOICE_WX)
else ()
    add_definitions(-DUIW_LINKLIB_IMPL_CHOICE_OWN)
endif ()

# dll export define
set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UIWRAP_LIBRARY)

set_target_properties(${TargetUIWrap} PROPERTIES DEBUG_POSTFIX "d")

configure_file(${TargetUIWrap}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetUIWrap}_build_config.h)
target_include_directories(${TargetUIWrap} PRIVATE ${CMAKE_BINARY_DIR})

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

set(do_auto_test	ON)

if (do_auto_test)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_custom_command(
            TARGET ${TargetUIWrapTest} POST_BUILD
            COMMAND qtdeploy_autodepends_bindbg_win_uiwraptest.bat
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
    else ()
        add_custom_command(
            TARGET ${TargetUIWrapTest} POST_BUILD
            COMMAND qtdeploy_autodepends_bin_win_uiwraptest.bat
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
    endif ()
    add_custom_command(
        TARGET ${TargetUIWrapTest} POST_BUILD
        COMMAND ${TargetUIWrapTest}
        )
endif ()
