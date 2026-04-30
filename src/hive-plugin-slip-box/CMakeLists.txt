file(GLOB_RECURSE PLUGIN_SLIP_BOX_SOURCES
        hive/plugins/slipbox/**/*.cpp
        hive/plugins/slipbox/*.cpp
)
set(CMAKE_UNITY_BUILD OFF)
add_library(hive_plugin_slip_box STATIC ${PLUGIN_SLIP_BOX_SOURCES})

# other
target_include_directories(hive_plugin_slip_box
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/hive-plugin-slip-box
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hive-essential
        ${CMAKE_SOURCE_DIR}/include/hive-model
        ${CMAKE_SOURCE_DIR}/include/hive-api
        ${CMAKE_SOURCE_DIR}/include/hive-util
)

include(FetchContent)

FetchContent_Declare(
        cmark
        GIT_REPOSITORY https://github.com/commonmark/cmark.git
        GIT_TAG master
)

FetchContent_MakeAvailable(cmark)

target_link_libraries(hive_plugin_slip_box
        PUBLIC
        hive_essential
        hive_model
        hive_api
        hive_util
        hive_db_sqlite
        PRIVATE cmark
)

target_precompile_headers(hive_plugin_slip_box PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
