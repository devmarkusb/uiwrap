file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
list(FILTER headers EXCLUDE REGEX "/config_gen\\.hpp$")

target_sources(
    mb.uiwrap
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}"
            FILES
                ${headers}
                "${PROJECT_SOURCE_DIR}/include/mb/uiwrap/uiwrapDEF.hpp"
                "${PROJECT_BINARY_DIR}/mb/uiwrap/config_gen.hpp"
                "${PROJECT_BINARY_DIR}/mb/uiwrap/uiwrap_build_config.hpp"
)
