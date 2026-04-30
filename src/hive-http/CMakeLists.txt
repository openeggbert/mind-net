file(GLOB_RECURSE HTTP_SOURCES
        hive/http/*.cpp
)

find_package(fmt REQUIRED)

add_library(hive_http STATIC ${HTTP_SOURCES})

# other
target_include_directories(hive_http
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/hive-http
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hive-essential
        ${CMAKE_SOURCE_DIR}/include/hive-util
        ${CMAKE_SOURCE_DIR}/include/hive-model
        ${CMAKE_SOURCE_DIR}/include/hive-api
)

target_link_libraries(hive_http
        PUBLIC
        nlohmann_json::nlohmann_json
        OpenSSL::Crypto
        pthread
        dl
        fmt::fmt
        hive_essential
        hive_util
        hive_model
        hive_api
)

target_precompile_headers(hive_http PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.hpp"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_crow.hpp"
        <asio.hpp>
        <fmt/format.h>
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
