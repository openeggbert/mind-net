file(GLOB_RECURSE UTIL_SOURCES
        hive/util/*.cpp
)

add_library(hive_util STATIC ${UTIL_SOURCES})

# util
target_include_directories(hive_util
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/hive-util
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hive-essential
)

target_link_libraries(hive_util
        PUBLIC
        hive_essential
)

target_precompile_headers(hive_util PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)