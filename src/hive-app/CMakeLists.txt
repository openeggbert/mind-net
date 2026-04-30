add_executable(hive_app hive/app/Main.cpp)

target_include_directories(hive_app PRIVATE
        ${CMAKE_SOURCE_DIR}/include/hive-http
)

find_library(JEMALLOC_LIB jemalloc)

if (JEMALLOC_LIB)
    message(STATUS "Found jemalloc: ${JEMALLOC_LIB}")
    target_link_libraries(hive_app PRIVATE ${JEMALLOC_LIB})
endif ()

target_link_libraries(hive_app PRIVATE
        hive_essential
        hive_util
        hive_model
        hive_orm
        hive_api
        hive_http
        hive_db_sqlite
        $<$<BOOL:${MINDNET_ENABLE_DICTIONARY_PLUGIN}>:hive_plugin_dictionary>
        $<$<BOOL:${MINDNET_ENABLE_SLIPBOX_PLUGIN}>:hive_plugin_slip_box>
        $<$<BOOL:${MINDNET_ENABLE_REPETITION_PLUGIN}>:hive_plugin_repetition>
        nlohmann_json::nlohmann_json
)

#if (MINDNET_ENABLE_DICTIONARY_PLUGIN)
#    target_link_libraries(hive_app PRIVATE
#            hive_plugin_dictionary
#    )
#endif ()
#
#if (MINDNET_ENABLE_SLIPBOX_PLUGIN)
#    target_link_libraries(hive_app PRIVATE
#            hive_plugin_slip_box
#    )
#endif ()
#
#if (MINDNET_ENABLE_REPETITION_PLUGIN)
#    target_link_libraries(hive_app PRIVATE
#            hive_plugin_repetition
#    )
#endif ()

target_compile_definitions(hive_app
        PUBLIC
        $<$<BOOL:${MINDNET_ENABLE_DICTIONARY_PLUGIN}>:MINDNET_ENABLE_DICTIONARY_PLUGIN>
        $<$<BOOL:${MINDNET_ENABLE_SLIPBOX_PLUGIN}>:MINDNET_ENABLE_SLIPBOX_PLUGIN>
        $<$<BOOL:${MINDNET_ENABLE_REPETITION_PLUGIN}>:MINDNET_ENABLE_REPETITION_PLUGIN>
)



target_precompile_headers(hive_app PRIVATE
        "${CMAKE_SOURCE_DIR}/include/pch/pch_std.hpp"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_json.hpp"
        "${CMAKE_SOURCE_DIR}/include/pch/pch_crow.hpp"
)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)