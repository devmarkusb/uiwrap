cmake_minimum_required(VERSION 3.30...4.3)

project(
    mb.uiwrap
    DESCRIPTION
        "UI-adjacent abstraction (filesystem, message boxes, settings, Qt helpers)"
    LANGUAGES CXX
    VERSION 0.1.0
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

add_subdirectory(devenv)
list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/devenv/cmake")
if(MB_UIWRAP_BUILD_TESTS)
    include(mb-devenv-ctest-root)
endif()
include(mb-devenv-defaults)
include(mb-devenv-install-library-config)
include(GNUInstallDirs)

# From fetched https://github.com/devmarkusb/pre-commit (see fetchcontent-lockfile.json).
mb_pre_commit_setup_project()

set(target mb.uiwrap)
set(target_basename uiwrap)
message(
    STATUS
    "${PROJECT_NAME} (library target: ${target}) version: "
    ${PROJECT_VERSION}
)

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

include(cmake/dependencies.cmake)

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
    ${PROJECT_BINARY_DIR}/uiwrap/config_gen.h
)

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    qt_standard_project_setup(REQUIRES 6.8)
endif()

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    qt_add_library(${target} SHARED)
else()
    add_library(${target} SHARED)
endif()
add_library(mb::uiwrap ALIAS ${target})

mb_devenv_set_target_lib_export_def(${target} UIWRAP_LIBRARY)

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    set(uiwrap_impl_choice_number 1)
elseif(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "wx")
    set(uiwrap_impl_choice_number 2)
else()
    set(uiwrap_impl_choice_number 0)
endif()

configure_file(
    ${target_basename}_build_config.h.in
    ${PROJECT_BINARY_DIR}/${target_basename}_build_config.h
)

target_include_directories(
    ${target}
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

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

target_link_libraries(${target} PUBLIC "$<BUILD_LOCAL_INTERFACE:mb::util>")

if(MB_DEVENV_LINUX)
    target_link_libraries(${target} PUBLIC dl)
endif()

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    target_link_libraries(${target} PUBLIC Qt::Core Qt::Gui Qt::Qml Qt::Widgets)
endif()

mb_devenv_set_target_defaults(${target})

add_subdirectory(include/uiwrap)
add_subdirectory(src)

if(NOT PROJECT_IS_TOP_LEVEL)
    set(MB_UIWRAP_INSTALL_CONFIG_FILE_PACKAGE OFF CACHE BOOL "" FORCE)
endif()

mb_devenv_install_library(${target})

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}
    CACHE INTERNAL
    "${target}: include directories"
    FORCE
)
