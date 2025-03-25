cmake_minimum_required(VERSION 3.15)

project(
  plusplusGameBoyEmulator
  VERSION 0.1.0
  LANGUAGES CXX
)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE GB_SRC ./src/*.cpp)
file(GLOB_RECURSE TEST_SRC ./tests/*.cpp)
find_package(SDL3 REQUIRED)

add_executable(ppGB main.cpp ${GB_SRC})
add_executable(pp_test ${TEST_SRC} ${GB_SRC})

target_include_directories(ppGB PUBLIC SDL3::SDL3)
target_link_libraries(ppGB PUBLIC SDL3::SDL3)

target_include_directories(pp_test PUBLIC SDL3::SDL3)
target_link_libraries(pp_test PUBLIC SDL3::SDL3)

add_test(NAME ALU_Tests COMMAND pp_test)
