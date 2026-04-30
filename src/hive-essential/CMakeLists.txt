# Get all .cpp files in this directory and subdirectories
file(GLOB_RECURSE ESSENTIAL_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/hive/essential/*.cpp"
)

find_package(fmt REQUIRED)

add_library(hive_essential STATIC
        ${ESSENTIAL_SOURCES}
)

target_include_directories(hive_essential PUBLIC
        ${CMAKE_SOURCE_DIR}/include/hive-essential
)

target_link_libraries(hive_essential PUBLIC
        fmt::fmt
)

target_precompile_headers(hive_essential PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
        <fmt/format.h>

)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)

