file(GLOB_RECURSE API_SOURCES
        hive/api/*.cpp
        hive/plugins/core/**/*.cpp
        hive/plugins/core/*.cpp
)

add_library(hive_api STATIC ${API_SOURCES})

# api
target_include_directories(hive_api
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/hive-api
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hive-essential
        ${CMAKE_SOURCE_DIR}/include/hive-util
        ${CMAKE_SOURCE_DIR}/include/hive-model
        ${CMAKE_SOURCE_DIR}/include/hive-orm
)

target_link_libraries(hive_api
        PUBLIC
        hive_essential
        hive_util
        hive_model
        hive_orm
        nlohmann_json::nlohmann_json
)

target_precompile_headers(hive_api PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)

