include_guard(GLOBAL)

if(NOT TARGET mb::util)
    find_package(mb.util CONFIG REQUIRED)
endif()

set(MB_UIWRAP_CONFIG_BACKEND_DEPENDENCIES "")

if(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "own")
    set(MB_UIWRAP_BOOST_MIN_VERSION
        "1.67"
        CACHE STRING
        "Minimum Boost for the \"own\" backend (header-only property_tree)."
    )
    find_package(Boost ${MB_UIWRAP_BOOST_MIN_VERSION} CONFIG REQUIRED)
    set(MB_UIWRAP_CONFIG_BACKEND_DEPENDENCIES
        "find_dependency(Boost ${MB_UIWRAP_BOOST_MIN_VERSION} CONFIG REQUIRED)"
    )
elseif(MB_UIWRAP_USE_IMPLEMENTATION STREQUAL "qt")
    mb_ul_include(qt.cmake)
    # Widgets contains Core and Gui; Widgets is used by QMessageBox. Qml is
    # used by QQmlApplicationEngine and the public Qt helper headers.
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Widgets)
    set(MB_UIWRAP_CONFIG_BACKEND_DEPENDENCIES
        "find_dependency(Qt6 REQUIRED COMPONENTS Core Gui Qml Widgets)"
    )
endif()
