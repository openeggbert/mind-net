file(GLOB_RECURSE PLUGIN_SUPER_MEMO_SOURCES
        mindnet/plugins/supermemo/**/*.cpp
        mindnet/plugins/supermemo/*.cpp
)

add_library(mind_net_plugin_super_memo STATIC ${PLUGIN_SUPER_MEMO_SOURCES})

# other
target_include_directories(mind_net_plugin_super_memo
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-super-memo
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
        ${CMAKE_SOURCE_DIR}/include/mind-net-model
        ${CMAKE_SOURCE_DIR}/include/mind-net-api
)


target_link_libraries(mind_net_plugin_super_memo
        PUBLIC
        mind_net_essential
        mind_net_model
        mind_net_api
)

target_precompile_headers(mind_net_plugin_super_memo PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
