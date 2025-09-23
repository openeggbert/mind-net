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
        ${CMAKE_SOURCE_DIR}/include/mind-net-util
        ${CMAKE_SOURCE_DIR}/include/mind-net-model
        ${CMAKE_SOURCE_DIR}/include/mind-net-api
)


target_link_libraries(mind_net_plugin_core
        PUBLIC
        mind_net_essential
        mind_net_util
        mind_net_model
        mind_net_api
)

target_precompile_headers(mind_net_plugin_core PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
