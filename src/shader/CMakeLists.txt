add_library(shader_lib color.cpp color.h light.cpp light.h material.cpp material.h)
target_link_libraries(shader_lib algebra_lib executer_lib)

if (altadore_tests)
  add_executable(shader_test color_test.cpp light_test.cpp material_test.cpp)
  target_link_libraries(shader_test gtest_main shader_lib)
  add_test(NAME shader_test COMMAND shader_test)
endif()
