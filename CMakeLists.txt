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

# Detect build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug) 
endif()

# SDL target
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
  declare_executable(ppGB platforms/sdl)

  include_directories(SYSTEM ${SDL2_INCLUDE_DIRS})

  # Static linking only for Release builds
  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building release: Enabling static linking")
    set(BUILD_SHARED_LIBS OFF)  # Ensure we prefer static libraries
    target_link_options(ppGB PRIVATE -static -static-libgcc -static-libstdc++)
  else()
    message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode: Using dynamic linking")
  endif()

  target_link_libraries(ppGB ppGB-core ${SDL2_LIBRARIES})
endif()

# Test target (always dynamically linked)
declare_executable(ppGB-test platforms/test)
target_link_libraries(ppGB-test ppGB-core)
