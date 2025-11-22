# tests/CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(catch2)

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(BuildTests ${TEST_SOURCES})
target_link_libraries(BuildTests PRIVATE LifeCore Catch2::Catch2WithMain)

set_target_properties(BuildTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

include(Catch)
catch_discover_tests(BuildTests)
