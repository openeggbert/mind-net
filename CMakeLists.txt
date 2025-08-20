cmake_minimum_required(VERSION 3.25.1)
project(mind_net)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Enable unit tests" ON)
option(MAKE_VERBOSE_MAKEFILE "" ON)

# Enable submodule dependencies
set(CCPSQLITE_DIR thirdparty/SQLiteCpp)
add_subdirectory(${CCPSQLITE_DIR})
add_subdirectory(thirdparty/nlohmann_json)

# Collect sources
file(GLOB_RECURSE ALL_SOURCES "src/*.cpp")

# Separate main.cpp from core sources
set(MAIN_SOURCE "src/mindnet/Main.cpp")
list(REMOVE_ITEM ALL_SOURCES ${MAIN_SOURCE})

# Add Crow dependency
set(CROW_DIR thirdparty/crow)

# Add Boost Asio dependency
set(ASIO_DIR thirdparty/asio)
set(ASIO_INCLUDE_DIR ${ASIO_DIR}/asio/include)
include_directories(${ASIO_INCLUDE_DIR})


# Core library target
add_library(mind_net_core STATIC
        ${ALL_SOURCES}
)

target_include_directories(mind_net_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/crow/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/asio/asio/include
)

target_link_libraries(mind_net_core PUBLIC
        SQLiteCpp
        sqlite3
        nlohmann_json::nlohmann_json
        pthread
        dl
)

# Executable using core library
add_executable(mind_net ${MAIN_SOURCE})
target_link_libraries(mind_net PRIVATE mind_net_core)

# GoogleTest setup
add_subdirectory(thirdparty/googletest)
enable_testing()

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(Tests ${TEST_SOURCES})

target_include_directories(Tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest/googletest/include
)

target_link_libraries(Tests PRIVATE mind_net_core gtest_main)

include(GoogleTest)
gtest_discover_tests(Tests)
