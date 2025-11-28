file(GLOB_RECURSE PLUGIN_SLIP_BOX_SOURCES
        mindnet/plugins/slipbox/**/*.cpp
        mindnet/plugins/slipbox/*.cpp
)
set(CMAKE_UNITY_BUILD OFF)
add_library(mind_net_plugin_slip_box STATIC ${PLUGIN_SLIP_BOX_SOURCES})

# other
target_include_directories(mind_net_plugin_slip_box
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-slip-box
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
        ${CMAKE_SOURCE_DIR}/include/mind-net-model
        ${CMAKE_SOURCE_DIR}/include/mind-net-api
        ${CMAKE_SOURCE_DIR}/include/mind-net-util
)

include(FetchContent)

FetchContent_Declare(
        cmark
        GIT_REPOSITORY https://github.com/commonmark/cmark.git
        GIT_TAG master
)

FetchContent_MakeAvailable(cmark)

target_link_libraries(mind_net_plugin_slip_box
        PUBLIC
        mind_net_essential
        mind_net_model
        mind_net_api
        mind_net_util
        mind_net_db_sqlite
        PRIVATE cmark
)

target_precompile_headers(mind_net_plugin_slip_box PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
