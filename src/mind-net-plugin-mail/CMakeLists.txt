file(GLOB_RECURSE PLUGIN_MAIL_SOURCES
        mindnet/plugins/mail/**/*.cpp
        mindnet/plugins/mail/*.cpp
)

add_library(mind_net_plugin_mail STATIC ${PLUGIN_MAIL_SOURCES})

# other
target_include_directories(mind_net_plugin_mail
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-plugin-mail
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
        ${CMAKE_SOURCE_DIR}/include/mind-net-model
        ${CMAKE_SOURCE_DIR}/include/mind-net-api
)


target_link_libraries(mind_net_plugin_mail
        PUBLIC
        mind_net_essential
        mind_net_model
        mind_net_api
)

target_precompile_headers(mind_net_plugin_mail PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
