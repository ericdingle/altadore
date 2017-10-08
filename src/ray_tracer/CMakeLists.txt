add_library(ray_tracer_lib ray_tracer.cpp ray_tracer.h ray_tracer_constants.cpp ray_tracer_constants.h)
target_link_libraries(ray_tracer_lib image_lib scene_lib shader_lib)

if (altadore_tests)
  add_executable(ray_tracer_test ray_tracer_test.cpp)
  target_link_libraries(ray_tracer_test gtest_main ray_tracer_lib)
  add_test(NAME ray_tracer_test COMMAND ray_tracer_test)
endif()
