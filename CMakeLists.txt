cmake_minimum_required(VERSION 3.15)

project(
  plusplusGameBoyEmulator
  VERSION 0.1.0
  LANGUAGES CXX
)

file(GLOB_RECURSE GB_SRC ./src/*.cpp)
find_package(SDL3 REQUIRED)

add_executable(ppGB main.cpp ${GB_SRC})

target_include_directories(ppGB PUBLIC SDL3::SDL3)
target_link_libraries(ppGB PUBLIC SDL3::SDL3)
