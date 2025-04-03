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

find_package(SDL2 REQUIRED)

add_executable(ppGB main.cpp ${GB_SRC})
add_executable(cpu_test ./tests/instruction_tests.cpp ${GB_SRC})
add_executable(ppu_test ./tests/ppu_tests.cpp ${GB_SRC})

target_include_directories(ppGB PUBLIC SDL2::SDL2)
target_link_libraries(ppGB PUBLIC SDL2::SDL2)

target_include_directories(cpu_test PUBLIC SDL2::SDL2)
target_link_libraries(cpu_test PUBLIC SDL2::SDL2)

target_include_directories(ppu_test PUBLIC SDL2::SDL2)
target_link_libraries(ppu_test PUBLIC SDL2::SDL2)

add_test(NAME CPU_TESTS COMMAND cpu_test)
add_test(NAME PPU_TESTS COMMAND ppu_test)
