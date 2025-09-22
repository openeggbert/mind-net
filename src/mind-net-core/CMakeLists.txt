# Get all .cpp files in this directory and subdirectories
file(GLOB_RECURSE MIND_NET_CORE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/mindnet/core/*.cpp"
)

add_library(mind_net_core STATIC
        ${MIND_NET_CORE_SOURCES}
)

target_include_directories(mind_net_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-core
)

target_link_libraries(mind_net_core PUBLIC
        SQLiteCpp # todo : move to db/sqlite
        sqlite3 # todo : move to db/sqlite
)
target_precompile_headers(mind_net_core PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_sqlite.h"
)

