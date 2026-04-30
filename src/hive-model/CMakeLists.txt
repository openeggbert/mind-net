file(GLOB_RECURSE MODEL_SOURCES
        hive/model/*.cpp
)

add_library(hive_model STATIC ${MODEL_SOURCES})

# model
target_include_directories(hive_model
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/hive-model
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hive-essential
        ${CMAKE_SOURCE_DIR}/include/hive-util
)

target_link_libraries(hive_model
        PUBLIC
        hive_essential
        hive_util
        nlohmann_json::nlohmann_json
)

target_precompile_headers(hive_model PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
