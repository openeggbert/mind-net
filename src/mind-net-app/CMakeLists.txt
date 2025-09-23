add_executable(mind_net_app mindnet/app/Main.cpp)

target_include_directories(mind_net_app PRIVATE
        ${CMAKE_SOURCE_DIR}/include/mind-net-http
)

target_link_libraries(mind_net_app PRIVATE
        mind_net_essential
        mind_net_util
        mind_net_model
        mind_net_orm
        mind_net_api
        mind_net_http
        mind_net_db_sqlite
        nlohmann_json::nlohmann_json
)

target_precompile_headers(mind_net_app PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.h"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_crow.h"
)