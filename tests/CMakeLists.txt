file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(Tests ${TEST_SOURCES})

configure_file( ${CMAKE_SOURCE_DIR}/configuration/hive.properties ${CMAKE_CURRENT_BINARY_DIR}/hive.properties COPYONLY )

target_include_directories(Tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Tests PRIVATE
        hive_essential
        hive_util
        hive_model
        hive_orm
        hive_api
        hive_http
        hive_db_sqlite
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(Tests)
