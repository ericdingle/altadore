{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'parser_lib',
    'type': 'static_library',
    'dependencies': [
      '../lexer/lexer.gyp:lexer_lib',
      '../../bonavista/string/string.gyp:string_lib',
    ],
    'sources': [
      'ast_node.cpp',
      'ast_node.h',
      'parser.cpp',
      'parser.h',
    ],
  }, {
    'target_name': 'parser_test',
    'type': 'executable',
    'dependencies': [
      'parser_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'parser_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ]
  }],
}
