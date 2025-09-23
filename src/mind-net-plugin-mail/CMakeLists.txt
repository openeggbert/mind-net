file(GLOB_RECURSE PLUGIN_MAIL_SOURCES
        mindnet/plugins/mail/**/*.cpp
)

add_library(mind_net_plugin_mail STATIC ${PLUGIN_MAIL_SOURCES})

# other
target_include_directories(mind_net_plugin_mail
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-mail
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)


target_link_libraries(mind_net_plugin_mail
        PUBLIC
        mind_net_essential
)

target_precompile_headers(mind_net_plugin_mail PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
