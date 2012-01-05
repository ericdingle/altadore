{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'calc_lib',
    'type': 'static_library',
    'dependencies': [
      '../executer/executer.gyp:executer_lib',
      '../lexer/lexer.gyp:lexer_lib',
      '../parser/parser.gyp:parser_lib',
    ],
    'sources': [
      'calc_executer.cpp',
      'calc_executer.h',
      'calc_lexer.cpp',
      'calc_lexer.h',
      'calc_parser.cpp',
      'calc_parser.h',
    ],
  }, {
    'target_name': 'calc',
    'type': 'executable',
    'dependencies': [
      'calc_lib',
    ],
    'sources': [
      'main.cpp',
    ],
  }, {
    'target_name': 'calc_test',
    'type': 'executable',
    'dependencies': [
      'calc_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'calc_lexer_test.cpp',
      'calc_parser_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
