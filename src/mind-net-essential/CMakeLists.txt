# Get all .cpp files in this directory and subdirectories
file(GLOB_RECURSE ESSENTIAL_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/mindnet/essential/*.cpp"
)

find_package(fmt REQUIRED)

add_library(mind_net_essential STATIC
        ${ESSENTIAL_SOURCES}
)

target_include_directories(mind_net_essential PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)

target_link_libraries(mind_net_essential PUBLIC
        fmt::fmt
)

target_precompile_headers(mind_net_essential PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
        <fmt/format.h>

)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)

