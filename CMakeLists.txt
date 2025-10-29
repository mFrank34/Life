cmake_minimum_required(VERSION 3.10)
project(LIFE LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Where your headers live
include_directories(${CMAKE_SOURCE_DIR}/Header)

# Collect all .cpp files automatically
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/Source/*.cpp"
)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})