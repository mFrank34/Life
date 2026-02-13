cmake_minimum_required(VERSION 3.14)
project(Life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

# Core library
add_library(LifeCore
        src/world/World.cpp

        src/world/structure/Cell.cpp
        src/world/structure/Chunk.cpp

        src/world/storage/Cache.cpp
        src/world/storage/Sparse.cpp
        src/world/storage/Unordered.cpp

        src/rules/Rules.cpp
)

target_include_directories(LifeCore PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)


# Executable
add_executable(Life
        src/app/App.cpp
        src/app/controller/Controller.cpp
        src/app/window/Interface.cpp
        src/world/Manager.cpp
        src/app/window/View.cpp
)

target_link_libraries(Life PRIVATE LifeCore)


# GTK4 (app only)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

target_include_directories(Life PRIVATE ${GTKMM_INCLUDE_DIRS})
target_compile_options(Life PRIVATE ${GTKMM_CFLAGS_OTHER})
target_link_directories(Life PRIVATE ${GTKMM_LIBRARY_DIRS})
target_link_libraries(Life PRIVATE ${GTKMM_LIBRARIES})


# Resources
file(COPY ${CMAKE_SOURCE_DIR}/src/resources
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Tests
if (BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif ()

# Install
install(TARGETS Life
        RUNTIME DESTINATION bin
)

install(DIRECTORY src/resources
        DESTINATION share/Life
)

# Packaging
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "Life")
set(CPACK_PACKAGE_VENDOR "Frank")
set(CPACK_PACKAGE_CONTACT "mffranks34@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Life GTK4 Application")
set(CPACK_PACKAGE_VERSION "1.0.0")

set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Applications")
set(CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_RPM_PACKAGE_REQUIRES "gtkmm4")

set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

if (WIN32)
    set(CPACK_GENERATOR "ZIP")
else ()
    set(CPACK_GENERATOR "RPM")
endif ()

include(CPack)
