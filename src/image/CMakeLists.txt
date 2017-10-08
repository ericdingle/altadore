add_library(image_lib bitmap.cpp bitmap.h)

if (altadore_tests)
  add_executable(image_test bitmap_test.cpp)
  target_link_libraries(image_test gtest_main image_lib)
  add_test(NAME image_test COMMAND image_test)
endif()
