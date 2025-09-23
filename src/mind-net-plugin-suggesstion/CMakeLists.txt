file(GLOB_RECURSE PLUGIN_SUGGESTION_SOURCES
        mindnet/plugins/suggestion/**/*.cpp
)

add_library(mind_net_plugin_suggestion STATIC ${PLUGIN_SUGGESTION_SOURCES})

# other
target_include_directories(mind_net_plugin_suggestion
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-suggestion
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)


target_link_libraries(mind_net_plugin_suggestion
        PUBLIC
        mind_net_essential
)

target_precompile_headers(mind_net_plugin_suggestion PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
