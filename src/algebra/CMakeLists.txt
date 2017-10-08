add_library(algebra_lib algebra_constants.cpp algebra_constants.h matrix4.cpp matrix4.h point3.cpp point3.h ray.cpp ray.h vector3.cpp vector3.h)
target_link_libraries(algebra_lib executer_lib)

if (altadore_tests)
  add_executable(algebra_test matrix4_test.cpp point3_test.cpp ray_test.cpp vector3_test.cpp)
  target_link_libraries(algebra_test gtest_main algebra_lib)
  add_test(NAME algebra_test COMMAND algebra_test)
endif()
