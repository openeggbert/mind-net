file(GLOB_RECURSE PLUGIN_CHAT_SOURCES
        mindnet/plugins/chat/**/*.cpp
)

add_library(mind_net_plugin_chat STATIC ${PLUGIN_CHAT_SOURCES})

# other
target_include_directories(mind_net_plugin_chat
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-chat
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)


target_link_libraries(mind_net_plugin_chat
        PUBLIC
        mind_net_essential
)

target_precompile_headers(mind_net_plugin_chat PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
