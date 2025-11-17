cmake_minimum_required(VERSION 3.10)

project(Life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Source files ----
file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/source/*.cpp"
)

# DEBUG: print how many files found
message(STATUS "Found source files: ${SOURCES}")

add_executable(Life ${SOURCES})

# ---- Include directory ----
target_include_directories(Life PRIVATE
        ${CMAKE_SOURCE_DIR}/source/include
)

# ---- GTK4 ----
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

target_include_directories(Life PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(Life PRIVATE ${GTK4_LIBRARY_DIRS})
target_compile_options(Life PRIVATE ${GTK4_CFLAGS_OTHER})
target_link_libraries(Life PRIVATE ${GTK4_LIBRARIES})
# ---- Warnings ----
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall -Wextra -Wpedantic -Wshadow -g
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE
            /W4 /WX /Zi
    )
endif ()

# ---- Output directory ----
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
