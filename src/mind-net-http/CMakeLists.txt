file(GLOB_RECURSE HTTP_SOURCES
        mindnet/http/*.cpp
)

add_library(mind_net_http STATIC ${HTTP_SOURCES})

# other
target_include_directories(mind_net_http
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/mind-net-http
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-essential
)


target_link_libraries(mind_net_http
        PUBLIC
        mind_net_essential
)

target_precompile_headers(mind_net_http PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
)
