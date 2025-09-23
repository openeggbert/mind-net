add_executable(mind_net_app mindnet/app/Main.cpp)

target_link_libraries(mind_net_app PRIVATE
        mind_net_essential
        mind_net_other
        nlohmann_json::nlohmann_json
)

target_precompile_headers(mind_net_app PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_crow.h"
)