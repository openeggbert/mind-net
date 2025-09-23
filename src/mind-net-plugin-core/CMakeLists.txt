file(GLOB_RECURSE PLUGIN_CORE_SOURCES
        mindnet/plugins/core/**/*.cpp
)

add_library(mind_net_plugin_core STATIC ${PLUGIN_CORE_SOURCES})

# other
target_include_directories(mind_net_plugin_core
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-core
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)


target_link_libraries(mind_net_plugin_core
        PUBLIC
        mind_net_essential
)

target_precompile_headers(mind_net_plugin_core PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
