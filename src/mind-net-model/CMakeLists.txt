file(GLOB_RECURSE MODEL_SOURCES
        mindnet/model/*.cpp
)

add_library(mind_net_model STATIC ${MODEL_SOURCES})

# model
target_include_directories(mind_net_model
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-model
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
        ${CMAKE_SOURCE_DIR}/include/mind-net-util
)


target_link_libraries(mind_net_model
        PUBLIC
        mind_net_essential
        mind_net_util
        nlohmann_json::nlohmann_json
)

target_precompile_headers(mind_net_model PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
