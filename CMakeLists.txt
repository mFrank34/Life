cmake_minimum_required(VERSION 3.10)
project(Life LANGUAGES CXX)

# Modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug if nothing is set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Include headers
include_directories(${CMAKE_SOURCE_DIR}/Header)

# Gather all source files from Body and Source
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/Body/*.cpp"
    "${CMAKE_SOURCE_DIR}/Source/*.cpp"
)

# Output binary to Bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Compiler warnings + debug symbols
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow -g
)
