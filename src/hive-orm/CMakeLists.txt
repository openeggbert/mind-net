file(GLOB_RECURSE ORM_SOURCES
        hive/orm/*.cpp
)

add_library(hive_orm STATIC ${ORM_SOURCES})

# other
target_include_directories(hive_orm
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/hive-orm
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hive-essential
        ${CMAKE_SOURCE_DIR}/include/hive-model
)

target_link_libraries(hive_orm
        PUBLIC
        hive_essential
        hive_model
)

target_precompile_headers(hive_orm PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
