file(GLOB_RECURSE API_SOURCES
        mindnet/api/*.cpp
        mindnet/plugins/core/**/*.cpp
        mindnet/plugins/core/*.cpp
)

add_library(mind_net_api STATIC ${API_SOURCES})

# api
target_include_directories(mind_net_api
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-api
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
        ${CMAKE_SOURCE_DIR}/include/mind-net-util
        ${CMAKE_SOURCE_DIR}/include/mind-net-model
        ${CMAKE_SOURCE_DIR}/include/mind-net-orm
)


target_link_libraries(mind_net_api
        PUBLIC
        mind_net_essential
        mind_net_util
        mind_net_model
        mind_net_orm
        nlohmann_json::nlohmann_json
)

target_precompile_headers(mind_net_api PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.h"
)
