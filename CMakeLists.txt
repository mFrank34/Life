cmake_minimum_required(VERSION 3.14)
project(Life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)  # Required for CTest integration
enable_testing()

# Core sources
file(GLOB_RECURSE CORE_SOURCES "${CMAKE_SOURCE_DIR}/source/*.cpp")
list(REMOVE_ITEM CORE_SOURCES "${CMAKE_SOURCE_DIR}/source/Main.cpp")

add_library(LifeCore ${CORE_SOURCES})
target_include_directories(LifeCore PUBLIC ${CMAKE_SOURCE_DIR}/source/include)

add_executable(Life source/App.cpp)
target_link_libraries(Life PRIVATE LifeCore)

# GTK4 setup
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

target_include_directories(LifeCore PUBLIC ${GTKMM_INCLUDE_DIRS})
target_compile_options(LifeCore PUBLIC ${GTKMM_CFLAGS_OTHER})
target_link_directories(LifeCore PUBLIC ${GTKMM_LIBRARY_DIRS})
target_link_libraries(LifeCore PUBLIC ${GTKMM_LIBRARIES})

set_target_properties(Life PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Add tests
add_subdirectory(tests)
