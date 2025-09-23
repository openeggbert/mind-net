file(GLOB_RECURSE DB_SQLITE_SOURCES
        mindnet/db/sqlite/*.cpp
)

add_library(mind_net_db_sqlite STATIC ${DB_SQLITE_SOURCES})

# db-sqlite
target_include_directories(mind_net_db_sqlite
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-db-sqlite
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)


target_link_libraries(mind_net_db_sqlite
        PUBLIC
        mind_net_essential
)

target_precompile_headers(mind_net_db_sqlite PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
