cmake_minimum_required(VERSION 3.25.1)
project(mind_net LANGUAGES CXX)

option(ALLOW_LEGACY_PLUGINS "Allow legacy plugins" ON)
option(MINDNET_ENABLE_DICTIONARY_PLUGIN "Enable Dictionary plugin" ON)

if(ALLOW_LEGACY_PLUGINS)
    option(MINDNET_ENABLE_SLIPBOX_PLUGIN "Enable Slipbox plugin" ON)
    option(MINDNET_ENABLE_REPETITION_PLUGIN "Enable Repetition plugin" OFF)
else()
    unset(MINDNET_ENABLE_SLIPBOX_PLUGIN CACHE)
    unset(MINDNET_ENABLE_REPETITION_PLUGIN CACHE)
endif()

message(STATUS "")
message(STATUS "Mind Net plugin configuration:")
message(STATUS "  Dictionary plugin : ${MINDNET_ENABLE_DICTIONARY_PLUGIN}")
message(STATUS "  Slipbox plugin    : ${MINDNET_ENABLE_SLIPBOX_PLUGIN}")
message(STATUS "  Repetition plugin : ${MINDNET_ENABLE_REPETITION_PLUGIN}")
message(STATUS "")

# ccache (if installed)
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
        message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

# Default build type
if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

# Optimization
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--gc-sections -s")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Unity build for all targets (faster builds)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Options
option(ENABLE_TESTS "Enable unit tests" ON)
option(MAKE_VERBOSE_MAKEFILE "" ON)

# External dependencies
find_package(OpenSSL REQUIRED)
add_subdirectory(thirdparty/SQLiteCpp)
add_subdirectory(thirdparty/nlohmann_json)
add_subdirectory(thirdparty/googletest)

# Crow/Asio include dirs
set(CROW_DIR thirdparty/crow)
set(ASIO_DIR thirdparty/asio)
include_directories(${CROW_DIR}/include)
include_directories(${ASIO_DIR}/asio/include)

# ===== Project modules =====
add_subdirectory(src/mind-net-essential)
add_subdirectory(src/mind-net-util)
add_subdirectory(src/mind-net-model)
add_subdirectory(src/mind-net-orm)
add_subdirectory(src/mind-net-api)
add_subdirectory(src/mind-net-db-sqlite)
add_subdirectory(src/mind-net-http)

if (MINDNET_ENABLE_DICTIONARY_PLUGIN)
    add_subdirectory(src/mind-net-plugin-dictionary)
endif ()

if (MINDNET_ENABLE_SLIPBOX_PLUGIN)
    add_subdirectory(src/mind-net-plugin-slip-box)
endif ()

if (MINDNET_ENABLE_REPETITION_PLUGIN)
    add_subdirectory(src/mind-net-plugin-repetition)
endif ()


add_subdirectory(src/mind-net-app)

# ===== Tests =====
if (ENABLE_TESTS)
        enable_testing()
        add_subdirectory(tests)
endif ()

configure_file(
        ${CMAKE_SOURCE_DIR}/configuration/mindnet.properties
        ${CMAKE_BINARY_DIR}/mindnet.properties
        COPYONLY
)
