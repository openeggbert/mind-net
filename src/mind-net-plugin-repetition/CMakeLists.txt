file(GLOB_RECURSE PLUGIN_REPETITION_SOURCES
        mindnet/plugins/repetition/**/*.cpp
        mindnet/plugins/repetition/*.cpp
)

add_library(mind_net_plugin_repetition STATIC ${PLUGIN_REPETITION_SOURCES})

# other
target_include_directories(mind_net_plugin_repetition
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-repetition
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
        ${CMAKE_SOURCE_DIR}/include/mind-net-model
        ${CMAKE_SOURCE_DIR}/include/mind-net-api
)


target_link_libraries(mind_net_plugin_repetition
        PUBLIC
        mind_net_essential
        mind_net_model
        mind_net_api
)

target_precompile_headers(mind_net_plugin_repetition PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
