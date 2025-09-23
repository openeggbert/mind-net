file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(Tests ${TEST_SOURCES})

target_include_directories(Tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Tests PRIVATE
        mind_net_essential
        mind_net_util
        mind_net_model
        mind_net_orm
        mind_net_api
        mind_net_http
        mind_net_db_sqlite
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(Tests)
