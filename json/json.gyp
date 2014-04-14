{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'json_lib',
    'type': 'static_library',
    'dependencies': [
      '../lexer/lexer.gyp:lexer_lib',
      '../parser/parser.gyp:parser_lib',
    ],
    'sources': [
      'json_lexer.cpp',
      'json_lexer.h',
      'json_parser.cpp',
      'json_parser.h',
    ],
  }, {
    'target_name': 'json',
    'type': 'executable',
    'dependencies': [
      'json_lib',
      '../../bonavista/file/file.gyp:file_lib',
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
      '../../bonavista/testing/testing.gyp:testing_main',
    ],
    'sources': [
      'json_lexer_test.cpp',
      'json_parser_test.cpp',
    ],
  }],
}
