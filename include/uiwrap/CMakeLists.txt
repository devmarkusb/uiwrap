file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
list(FILTER headers EXCLUDE REGEX "/config_gen\\.h$")

target_sources(
    mb.uiwrap
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}"
            FILES
                ${headers}
                "${PROJECT_SOURCE_DIR}/include/uiwrapDEF.h"
                "${PROJECT_BINARY_DIR}/uiwrap/config_gen.h"
                "${PROJECT_BINARY_DIR}/uiwrap_build_config.h"
)
