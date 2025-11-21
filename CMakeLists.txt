cmake_minimum_required(VERSION 3.10)

project(Life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Source files (exclude main.cpp) ----
file(GLOB_RECURSE CORE_SOURCES "${CMAKE_SOURCE_DIR}/source/*.cpp")
list(REMOVE_ITEM CORE_SOURCES "${CMAKE_SOURCE_DIR}/source/Main.cpp")

# ---- Shared library ----
add_library(LifeCore ${CORE_SOURCES})
target_include_directories(LifeCore PUBLIC ${CMAKE_SOURCE_DIR}/source/include)

# ---- Executable: Life ----
add_executable(Life source/Main.cpp)
target_link_libraries(Life PRIVATE LifeCore)

# ---- GTK4 ----
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

target_include_directories(Life PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(Life PRIVATE ${GTK4_LIBRARY_DIRS})
target_compile_options(Life PRIVATE ${GTK4_CFLAGS_OTHER})
target_link_libraries(Life PRIVATE ${GTK4_LIBRARIES})

# ---- Output directory ----
set_target_properties(Life PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# ---- Add tests ----
enable_testing()
add_subdirectory(tests)
