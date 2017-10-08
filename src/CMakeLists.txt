cmake_minimum_required (VERSION 2.8.2)

project(altadore)

# Chaparral.
configure_file(Chaparral.cmake third_party/chaparral/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/chaparral" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/chaparral" )
set(chaparral_tests OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/chaparral/src")

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
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Include relative to root source dir.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(algebra)
add_subdirectory(image)
add_subdirectory(ray_tracer)
add_subdirectory(scene)
add_subdirectory(scene_interp)
add_subdirectory(shader)
add_subdirectory(shape)

add_executable(altadore main.cpp)
target_link_libraries(altadore ray_tracer_lib scene_interp_lib)
