file(GLOB_RECURSE OTHER_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/api/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/db/**/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/http/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins/**/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cpp
)

add_library(mind_net_other STATIC ${OTHER_SOURCES})

target_include_directories(mind_net_other PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(mind_net_other
        PUBLIC
        SQLiteCpp sqlite3
        nlohmann_json::nlohmann_json
        OpenSSL::Crypto
        pthread
        dl
        mind_net_core
)

target_precompile_headers(mind_net_other PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_sqlite.h"
)
