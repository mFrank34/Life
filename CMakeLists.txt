cmake_minimum_required(VERSION 3.14)
project(Life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_TESTS "Build tests" ON)

# Core sources
file(GLOB_RECURSE CORE_SOURCES "${CMAKE_SOURCE_DIR}/source/*.cpp")
list(REMOVE_ITEM CORE_SOURCES "${CMAKE_SOURCE_DIR}/source/App.cpp")

# Copy CSS resources to the runtime directory (dev builds)
file(COPY ${CMAKE_SOURCE_DIR}/source/resources
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(LifeCore ${CORE_SOURCES})
target_include_directories(LifeCore PUBLIC ${CMAKE_SOURCE_DIR}/source/include)

add_executable(Life source/App.cpp)
target_link_libraries(Life PRIVATE LifeCore)

# GTK4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

target_include_directories(LifeCore PUBLIC ${GTKMM_INCLUDE_DIRS})
target_compile_options(LifeCore PUBLIC ${GTKMM_CFLAGS_OTHER})
target_link_directories(LifeCore PUBLIC ${GTKMM_LIBRARY_DIRS})
target_link_libraries(LifeCore PUBLIC ${GTKMM_LIBRARIES})

# Tests (optional)
if(BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install
install(TARGETS Life
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/source/resources
    DESTINATION share/Life
)

# Packaging (CPack)
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "Life")  # RPM-safe
set(CPACK_PACKAGE_VENDOR "Frank")
set(CPACK_PACKAGE_CONTACT "mffranks34@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Life GTK4 Application")
set(CPACK_PACKAGE_VERSION "1.0.0")

set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Applications")
set(CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_RPM_PACKAGE_REQUIRES "gtkmm4")

set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "RPM")
endif()

include(CPack)
