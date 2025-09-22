cmake_minimum_required(VERSION 3.25.1)
project(mind_net LANGUAGES CXX)

# Default build type
if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

# Optimization
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--gc-sections -s")

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(ENABLE_TESTS "Enable unit tests" ON)
option(MAKE_VERBOSE_MAKEFILE "" ON)

# External dependencies
find_package(OpenSSL REQUIRED)
add_subdirectory(thirdparty/SQLiteCpp)
add_subdirectory(thirdparty/nlohmann_json)
add_subdirectory(thirdparty/googletest)

# Add Crow and Asio includes globally
set(CROW_DIR thirdparty/crow)
set(ASIO_DIR thirdparty/asio)
include_directories(${CROW_DIR}/include)
include_directories(${ASIO_DIR}/asio/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/jwt-cpp/include)

# Our modules
add_subdirectory(src/mindnet/core)
add_subdirectory(src/mindnet/other)
add_subdirectory(src/mindnet/app)

# Tests
if (ENABLE_TESTS)
        enable_testing()
        add_subdirectory(tests)
endif ()
