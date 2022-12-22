cmake_minimum_required(VERSION 3.8.2)

include(${CMAKE_CURRENT_LIST_DIR}/cmake_util/toolib.cmake)

include(${ul_CMAKE_DIRS}/cmake_util.cmake)

ul_include(buildnr.cmake)
project(uiwrap VERSION 0.1.${BUILD_NR}.0)
message(STATUS "${PROJECT_NAME} version: " ${PROJECT_VERSION})
set(target ${PROJECT_NAME})

if (MSVC)
    # not good, I only intended to suppress this for Qt headers but didn't find a way
    add_definitions(-wd4127)
endif ()

ul_include(defaults.cmake)

ul_include(fetch-clangformat-repo.cmake)
ul_include(clang-format.cmake)
ul_include(githooks.cmake)

set(uiwrap_USE_IMPLEMENTATION "own" CACHE STRING "Choose the desired UI implementation out of: qt, wx, ... or own (the default)")

ul_include(boost.cmake)
ul_find_boost(${UL_USE_BOOST_ver1} ${UL_USE_BOOST_ver2} ${UL_USE_BOOST_ver3} "")

file(GLOB_RECURSE target_INCS "include/*.h")
file(GLOB_RECURSE target_SRC_INCS "src/*.h")

set(target_COMMON_SOURCES
    src/filesys/filesys.cpp
    src/linklib/linklib.cpp
    src/messagebox/messagebox.cpp
    src/programsettings/programsettings.cpp
    )

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    ul_include(qt5.cmake)
    # Widgets contains (a.o.t.) Core and Gui; and is needed fo QMessageBox
    # Qml is needed e.g. for QQmlApplicationEngine (Quick would do also - could not find detailed infos yet, which to choose)
    find_package(Qt5 COMPONENTS Core Widgets Qml)

    set(target_IMPL_SOURCES
        include/uiwrap/GUI/impl_Qt/qmlextlib.cpp
        src/filesys/impl_Qt/filesys_Qt.cpp
        src/messagebox/impl_Qt/MessageBox_Qt.cpp
        src/programsettings/impl_Qt/ProgSettings_Qt.cpp
        )

elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set(target_IMPL_SOURCES ""
        )

else ()
    set(target_IMPL_SOURCES
        src/filesys/impl_/filesys_.cpp
        src/messagebox/impl_/messagebox_.cpp
        )

endif ()

add_library(${target} SHARED
    ${target_COMMON_SOURCES}
    ${target_IMPL_SOURCES}
    ${target_INCS}
    ${target_SRC_INCS}
    )

ul_set_target_lib_export_def(${target} UIWRAP_LIBRARY)

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(uiwrap_impl_choice_number 1)
elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set(uiwrap_impl_choice_number 2)
else ()
    set(uiwrap_impl_choice_number 0)
endif ()

configure_file(${target}_build_config.h.in ${CMAKE_BINARY_DIR}/${target}_build_config.h)
target_include_directories(${target} PUBLIC ${CMAKE_BINARY_DIR})

target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PRIVATE src)
target_include_directories(${target} SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_include_directories(${target} SYSTEM PUBLIC ${Qt5Qml_INCLUDE_DIRS})
    target_include_directories(${target} SYSTEM PUBLIC ${Qt5Widgets_INCLUDE_DIRS})
endif ()
target_include_directories(${target} PUBLIC ${ul_INCLUDE_DIRS})
target_include_directories(${target} PUBLIC ${toolib_INCLUDE_DIRS})

target_link_libraries(${target} PUBLIC toolib)

if (${UL_LINUX})
    target_link_libraries(${target} PUBLIC dl)
endif ()

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_link_libraries(${target} PUBLIC Qt5::Qml)
    target_link_libraries(${target} PUBLIC Qt5::Widgets)
endif ()

ul_set_target_defaults(${target})

set(${target}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${target}: include directories" FORCE)


################################################################################################

if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}Test)

set(targetTest_SOURCES
        src/filesys/filesys.test.cpp
    )

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(targetTest_IMPL_SOURCES
            src/fundamental_types/impl_Qt/convert_Qt.test.cpp
            src/GUI/impl_Qt/cppmodel2qml.test.cpp
            src/GUI/impl_Qt/qmlextlib.test.cpp
            src/string/impl_Qt/urlstring_util_Qt.test.cpp
    )
elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
    set(targetTest_IMPL_SOURCES "")
else ()
    set(targetTest_IMPL_SOURCES
        src/messagebox/impl_/messagebox_.test.cpp
        )
endif ()

add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${targetTest_IMPL_SOURCES}
    ${target_INCS}
    )

target_include_directories(${targetTest} PRIVATE ${uiwrap_INCLUDE_DIRS})

target_link_libraries(${targetTest} PRIVATE gtest_main ${target})

ul_set_target_defaults(${targetTest})

ul_include(qt_deploy.cmake)
ul_qt_deploy(${targetTest} "")

ul_run_target_postbuild(${targetTest})
