file(GLOB_RECURSE HTTP_SOURCES
        mindnet/http/*.cpp
)

find_package(fmt REQUIRED)

add_library(mind_net_http STATIC ${HTTP_SOURCES})

# other
target_include_directories(mind_net_http
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-http
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
        ${CMAKE_SOURCE_DIR}/include/mind-net-util
        ${CMAKE_SOURCE_DIR}/include/mind-net-model
        ${CMAKE_SOURCE_DIR}/include/mind-net-api
)

target_link_libraries(mind_net_http
        PUBLIC
        nlohmann_json::nlohmann_json
        OpenSSL::Crypto
        pthread
        dl
        fmt::fmt
        mind_net_essential
        mind_net_util
        mind_net_model
        mind_net_api
)

target_precompile_headers(mind_net_http PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.hpp"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_crow.hpp"
        <asio.hpp>
        <fmt/format.h>
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
