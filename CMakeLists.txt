cmake_minimum_required(VERSION 3.10)

# --- Project setup ---
project(Life LANGUAGES CXX)

# Use modern C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# --- Source files ---
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/body/*.cpp"
    "${CMAKE_SOURCE_DIR}/source/*.cpp"
)

# --- Create executable ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- Include directories ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# --- GTK4 Integration ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

target_include_directories(${PROJECT_NAME} PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${GTK4_LIBRARY_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${GTK4_CFLAGS_OTHER})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK4_LIBRARIES})
# --- End GTK4 Section ---

# --- Compiler warnings and debug symbols ---
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic -Wshadow -g
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4 /WX /Zi
    )
endif()

# --- Output binary directory ---
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
