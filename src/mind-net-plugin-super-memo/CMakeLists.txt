file(GLOB_RECURSE PLUGIN_SUPER_MEMO_SOURCES
        mindnet/plugins/supermemo/**/*.cpp
)

add_library(mind_net_plugin_super_memo STATIC ${PLUGIN_SUPER_MEMO_SOURCES})

# other
target_include_directories(mind_net_plugin_super_memo
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-super-memo
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)


target_link_libraries(mind_net_plugin_super_memo
        PUBLIC
        mind_net_essential
)

target_precompile_headers(mind_net_plugin_super_memo PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
