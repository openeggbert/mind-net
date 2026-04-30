file(GLOB_RECURSE PLUGIN_REPETITION_SOURCES
        hive/plugins/repetition/**/*.cpp
        hive/plugins/repetition/*.cpp
)

add_library(hive_plugin_repetition STATIC ${PLUGIN_REPETITION_SOURCES})

# other
target_include_directories(hive_plugin_repetition
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/hive-plugin-repetition
        ${CMAKE_SOURCE_DIR}/include/hive-plugin-slip-box
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hive-essential
        ${CMAKE_SOURCE_DIR}/include/hive-model
        ${CMAKE_SOURCE_DIR}/include/hive-api
)

target_link_libraries(hive_plugin_repetition
        PUBLIC
        hive_plugin_slip_box
        hive_essential
        hive_model
        hive_api
)

target_precompile_headers(hive_plugin_repetition PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
