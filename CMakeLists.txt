cmake_minimum_required(VERSION 3.12)
project(GameBoyEmulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-ggdb -pedantic-errors -Wall -Weffc++ -Wextra")

# Find SDL2 - this requires SDL2 to be installed with CMake config files
find_package(SDL2 REQUIRED)
find_package(Catch2 3 REQUIRED)

# Include directories for your headers and third-party
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party
)

# Source files
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

# Main executable
add_executable(ppGB-emu ${SRC_FILES} main.cpp)
target_link_libraries(ppGB-emu PRIVATE SDL2::SDL2)

# Tests executable (with Catch2 and all source, for integration tests etc)
add_executable(tests EXCLUDE_FROM_ALL ${TEST_SOURCES} ${SRC_FILES})
target_compile_definitions(tests PRIVATE CATCH_CONFIG_MAIN)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain SDL2::SDL2)

# Optionally, if SDL2::SDL2main is needed (Windows), add it:
if(TARGET SDL2::SDL2main)
    target_link_libraries(ppGB-emu PRIVATE SDL2::SDL2main)
    target_link_libraries(tests PRIVATE SDL2::SDL2main)
endif()

# Enable CTest for running tests
enable_testing()
add_test(NAME AllTests COMMAND tests)
