file(GLOB_RECURSE PLUGIN_SLIP_BOX_SOURCES
        mindnet/plugins/slipbox/**/*.cpp
        mindnet/plugins/slipbox/*.cpp
)

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


target_link_libraries(mind_net_plugin_slip_box
        PUBLIC
        mind_net_essential
        mind_net_model
        mind_net_api
        mind_net_util
        mind_net_db_sqlite
)

target_precompile_headers(mind_net_plugin_slip_box PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
