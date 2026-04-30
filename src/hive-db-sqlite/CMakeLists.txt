file(GLOB_RECURSE DB_SQLITE_SOURCES
        hive/db/sqlite/*.cpp
)

add_library(hive_db_sqlite STATIC ${DB_SQLITE_SOURCES})

# db-sqlite
target_include_directories(hive_db_sqlite
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/hive-db-sqlite
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hive-essential
        ${CMAKE_SOURCE_DIR}/include/hive-util
        ${CMAKE_SOURCE_DIR}/include/hive-api
        ${CMAKE_SOURCE_DIR}/include/hive-model
)

target_link_libraries(hive_db_sqlite
        PUBLIC
        SQLiteCpp sqlite3
        hive_essential
        hive_util
        hive_api
        hive_model
)

target_precompile_headers(hive_db_sqlite PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
