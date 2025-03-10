cmake_minimum_required(VERSION 3.15)

project(
  plusplusGameBoyEmulator
  VERSION 0.1.0
  LANGUAGES CXX
)

file(GLOB_RECURSE GB_SRC ./src/*.cpp)

add_executable(ppGB main.cpp ${GB_SRC})

