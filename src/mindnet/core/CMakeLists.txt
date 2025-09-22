# Get all .cpp files in this directory and subdirectories
file(GLOB_RECURSE MIND_NET_CORE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(mind_net_core STATIC
        ${MIND_NET_CORE_SOURCES}
)

target_include_directories(mind_net_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)


target_link_libraries(mind_net_core PUBLIC
        SQLiteCpp # todo : move to db/sqlite
        sqlite3 # todo : move to db/sqlite
)
