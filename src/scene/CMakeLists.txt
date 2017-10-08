add_library(scene_lib scene_node.cpp scene_node.h shape_node.cpp shape_node.h transform_node.cpp transform_node.h)
target_link_libraries(scene_lib shader_lib shape_lib)

if (altadore_tests)
  add_executable(scene_test shape_node_test.cpp transform_node_test.cpp)
  target_link_libraries(scene_test gtest_main scene_lib)
  add_test(NAME scene_test COMMAND scene_test)
endif()
