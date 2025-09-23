file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(Tests ${TEST_SOURCES})

target_include_directories(Tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Tests PRIVATE
        mind_net_essential
        mind_net_other
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(Tests)
