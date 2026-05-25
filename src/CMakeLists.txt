file(GLOB_RECURSE private_headers CONFIGURE_DEPENDS "*.h")

set(sources
    filesys/filesys.cpp
    linklib/linklib.cpp
    messagebox/messagebox.cpp
    programsettings/programsettings.cpp
)

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    list(
        APPEND sources
        "${PROJECT_SOURCE_DIR}/include/mb/uiwrap/GUI/impl_Qt/qmlextlib.cpp"
        filesys/impl_Qt/filesys_Qt.cpp
        messagebox/impl_Qt/MessageBox_Qt.cpp
        programsettings/impl_Qt/ProgSettings_Qt.cpp
    )
elseif(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "wx")
    # The wx backend is reserved for a future implementation.
else()
    list(
        APPEND sources
        filesys/impl_/filesys_.cpp
        messagebox/impl_/messagebox_.cpp
        programsettings/impl_/ProgSettings_.cpp
    )
endif()

target_sources(mb.uiwrap PRIVATE ${sources} ${private_headers})
target_include_directories(mb.uiwrap PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

include(mb-devenv-compile-check)
mb_devenv_add_compile_check(
    mb.uiwrap.compile-check
    PRIVATE mb::uiwrap
    HEADER "mb/uiwrap/filesys/filesys.h"
)

if(NOT MB_UIWRAP_BUILD_TESTS)
    return()
endif()

include(mb-devenv-googletest)

set(test_sources filesys/filesys.test.cpp)

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    list(
        APPEND test_sources
        fundamental_types/impl_Qt/convert_Qt.test.cpp
        GUI/impl_Qt/cppmodel2qml.test.cpp
        GUI/impl_Qt/qmlextlib.test.cpp
        string/impl_Qt/urlstring_util_Qt.test.cpp
    )
elseif(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "wx")
    # The wx backend has no tests until the implementation is wired.
else()
    list(APPEND test_sources messagebox/impl_/messagebox_.test.cpp)
endif()

add_executable(mb.uiwrap.test)
target_sources(mb.uiwrap.test PRIVATE ${test_sources})
target_include_directories(mb.uiwrap.test PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(mb.uiwrap.test PRIVATE GTest::gtest_main mb::uiwrap)
mb_devenv_set_target_essentials(mb.uiwrap.test)
gtest_discover_tests(mb.uiwrap.test)

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    mb_ul_include(qt-deploy.cmake)
    mb_ul_qt_deploy(mb.uiwrap.test "")
endif()
