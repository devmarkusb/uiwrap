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

project(uiwrap VERSION 0.1.0.0)
set(TargetUIWrap "uiwrap")

set(OutputDir_bin ${CMAKE_SOURCE_DIR}/bin)
set(OutputDir_lib ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OutputDir_bin})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OutputDir_lib})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OutputDir_lib})

# workaround for msvc, which adds additional subdirs per config
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
foreach (OutputConfig ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OutputConfig} OutputConfig)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_bin})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_lib})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_lib})
endforeach ()
endif ()

# this disturbs linking to gtest, expects d there as well...
# so better handle this via set_target_properties
#set(CMAKE_DEBUG_POSTFIX "d")

# BITS will be 32, 64, ...
math(EXPR BITS "8 * ${CMAKE_SIZEOF_VOID_P}")

# following 2 lines don't work as expected, why?
#set_property(GLOBAL PROPERTY CXX_STANDARD 11)
#set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-std=c++11")
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/W4")
    add_compile_options("/Zo")	# disable edit and continue debugging (would collide e.g. with SafeSEH)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options("-std=c++11")
    add_compile_options("-Wall")
endif ()


add_library(${TargetUIWrap} SHARED
    src/filesys/impl_/FileSys_.cpp
    src/filesys/filesys.cpp
    src/linklib/linklib.cpp
    src/programsettings/programsettings.cpp
)

if (WIN32)
set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UIW_LINKLIB_IMPL_CHOICE_OWN)

# dll export define
set_property(TARGET ${TargetUIWrap} APPEND PROPERTY COMPILE_DEFINITIONS UIWRAP_LIBRARY)

set_target_properties(${TargetUIWrap} PROPERTIES DEBUG_POSTFIX "d")

set(BOOST_ROOT "c:/projects/externlibs/boost_1_56_0")
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.56.0)

if (NOT Toolib_INCLUDE_DIRS)
set(Toolib_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/../Toolib)
endif ()
if (NOT Toolib_LIBRARY_DIRS)
set(Toolib_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/../Toolib/lib)
endif ()

find_library(Toolib Toolib HINTS ${Toolib_LIBRARY_DIRS})
#add_library(Toolib SHARED IMPORTED)
#set_property(TARGET Toolib PROPERTY IMPORTED_LOCATION ${Toolib_LIBRARY_DIRS}/Toolib.lib)
#find_package(Toolib)

target_include_directories(${TargetUIWrap} PUBLIC .)
target_include_directories(${TargetUIWrap} PUBLIC include)
target_include_directories(${TargetUIWrap} PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(${TargetUIWrap} PUBLIC ${Toolib_INCLUDE_DIRS})

target_link_libraries(${TargetUIWrap} PUBLIC ${Toolib})


################################################################################################

set(TargetUIWrapTest "UIWrapTest")

add_executable(${TargetUIWrapTest}
    test/src/filesys/filesys.test.cpp
)

if (WIN32)
set_property(TARGET ${TargetUIWrapTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetUIWrapTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT "c:/projects/externlibs/gtest-1.7.0")
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${TargetUIWrapTest} PRIVATE ${GTEST_INCLUDE_DIRS})

MESSAGE("gtest both: ${GTEST_BOTH_LIBRARIES}")
target_link_libraries(${TargetUIWrapTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TargetUIWrapTest} PRIVATE ${TargetUIWrap})


add_custom_command(
  TARGET ${TargetUIWrapTest} POST_BUILD
  COMMAND ${TargetUIWrapTest}
)

