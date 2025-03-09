cmake_minimum_required(VERSION 3.15)

project(
  plusplusGameBoyEmulator
  VERSION 0.1.0
  LANGUAGES CXX
)

add_executable(ppGB main.cpp src/memory.cpp src/cpu.cpp)

