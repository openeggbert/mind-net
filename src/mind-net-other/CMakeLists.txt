file(GLOB_RECURSE OTHER_SOURCES
        mindnet/api/*.cpp
        mindnet/db/**/*.cpp
        mindnet/http/*.cpp
        mindnet/orm/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mindnet/model/*.cpp
        mindnet/plugins/**/*.cpp
        mindnet/util/*.cpp
)

add_library(mind_net_other STATIC ${OTHER_SOURCES})

# other
target_include_directories(mind_net_other
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-other
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)


target_link_libraries(mind_net_other
        PUBLIC
        SQLiteCpp sqlite3
        nlohmann_json::nlohmann_json
        OpenSSL::Crypto
        pthread
        dl
        mind_net_essential
)

target_precompile_headers(mind_net_other PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_sqlite.h"
)
