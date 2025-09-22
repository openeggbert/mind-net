add_executable(mind_net_app Main.cpp)

target_link_libraries(mind_net_app PRIVATE
        mind_net_core
        mind_net_other
)

target_precompile_headers(mind_net_app PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_crow.h"
)