file(GLOB_RECURSE ORM_SOURCES
        mindnet/orm/*.cpp
)

add_library(mind_net_orm STATIC ${ORM_SOURCES})

# other
target_include_directories(mind_net_orm
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-orm
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
        ${CMAKE_SOURCE_DIR}/include/mind-net-model
)


target_link_libraries(mind_net_orm
        PUBLIC
        mind_net_essential
        mind_net_model
)

target_precompile_headers(mind_net_orm PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
