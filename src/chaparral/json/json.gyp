{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'json_lib',
    'type': 'static_library',
    'dependencies': [
      '../lexer/lexer.gyp:lexer_lib',
      '../parse/parse.gyp:parse_lib',
    ],
    'sources': [
      'array_operator.cpp',
      'array_operator.h',
      'json_lexer.cpp',
      'json_lexer.h',
      'json_parser.cpp',
      'json_parser.h',
      'object_operator.cpp',
      'object_operator.h',
    ],
  }, {
    'target_name': 'json',
    'type': 'executable',
    'dependencies': [
      'json_lib',
      '../../bonavista/io/io.gyp:io_lib',
    ],
    'sources': [
      'main.cpp',
    ],
  }, {
    'target_name': 'json_test',
    'type': 'executable',
    'dependencies': [
      'json_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'json_lexer_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
