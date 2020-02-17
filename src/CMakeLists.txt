cmake_minimum_required (VERSION 3.13.4)

project(altadore)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include relative to root source dir.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Chaparral.
configure_file(Chaparral.cmake third_party/chaparral/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/chaparral" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/chaparral" )
set(chaparral_tests OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/chaparral/src)

option(altadore_tests "Whether to create test targets." ON)
if (altadore_tests)
  enable_testing()

  # GoogleTest.
  configure_file(GoogleTest.cmake third_party/googletest/CMakeLists.txt)
  execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/googletest" )
  execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/googletest" )
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  set(BUILD_GTEST ON CACHE BOOL "" FORCE)
  add_subdirectory(third_party/googletest)
endif()

add_subdirectory(algebra)
add_subdirectory(image)
add_subdirectory(ray_tracer)
add_subdirectory(scene)
add_subdirectory(scene_interp)
add_subdirectory(shader)
add_subdirectory(shape)

add_executable(altadore main.cpp)
target_link_libraries(altadore ray_tracer_lib scene_interp_lib)
