add_library(shape_lib cube.cpp cube.h shape.h shape_constants.cpp shape_constants.h sphere.cpp sphere.h)
target_link_libraries(shape_lib algebra_lib)

if (altadore_tests)
  add_executable(shape_test cube_test.cpp sphere_test.cpp)
  target_link_libraries(shape_test gtest_main shape_lib)
  add_test(NAME shape_test COMMAND shape_test)
endif()
