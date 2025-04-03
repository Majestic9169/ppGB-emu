cmake_minimum_required(VERSION 3.15)
list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(add_warnings)
include(add_sources)

project(ppGB)

add_definitions(-std=c++17)
add_warnings()

declare_library(ppGB-core src)

# SDL target
find_package(SDL2)

if (SDL2_FOUND)
  declare_executable(ppGB platforms/sdl)
  include_directories(SYSTEM ${SDL2_INCLUDE_DIRS})
  target_link_libraries(ppGB ppGB-core ${SDL2_LIBRARIES})
endif()

# Test target
declare_executable(ppGB-test platforms/test)
target_link_libraries(ppGB-test ppGB-core)
