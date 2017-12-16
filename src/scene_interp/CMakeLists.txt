add_library(scene_interp_lib scene_executer.cpp scene_executer.h scene_lexer.cpp scene_lexer.h
            scene_object.h scene_parser.cpp scene_parser.h)
target_link_libraries(scene_interp_lib algebra_lib executer_lib lexer_lib parser_lib)

if (altadore_tests)
  add_executable(scene_interp_test scene_executer_test.cpp scene_lexer_test.cpp scene_parser_test.cpp)
  target_link_libraries(scene_interp_test gtest_main scene_interp_lib)
  add_test(NAME scene_interp_test COMMAND scene_interp_test)
endif()
