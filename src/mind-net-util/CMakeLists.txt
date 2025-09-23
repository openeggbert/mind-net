file(GLOB_RECURSE UTIL_SOURCES
        mindnet/util/*.cpp
)

add_library(mind_net_util STATIC ${UTIL_SOURCES})

# util
target_include_directories(mind_net_util
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-util
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)

target_link_libraries(mind_net_util
        PUBLIC
        mind_net_essential
)

target_precompile_headers(mind_net_util PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
