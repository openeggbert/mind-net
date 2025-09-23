file(GLOB_RECURSE PLUGIN_SLIP_BOX_SOURCES
        mindnet/plugins/slipbox/**/*.cpp
)

add_library(mind_net_plugin_slip_box STATIC ${PLUGIN_SLIP_BOX_SOURCES})

# other
target_include_directories(mind_net_plugin_slip_box
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-slip-box
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)


target_link_libraries(mind_net_plugin_slip_box
        PUBLIC
        mind_net_essential
)

target_precompile_headers(mind_net_plugin_slip_box PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
