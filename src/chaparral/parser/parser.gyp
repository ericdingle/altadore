{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'parser_lib',
    'type': 'static_library',
    'dependencies': [
      '../lexer/lexer.gyp:lexer_lib',
      '../../bonavista/logging/logging.gyp:logging_lib',
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
      '../../bonavista/testing/testing.gyp:testing_main',
    ],
    'sources': [
      'parser_test.cpp',
    ]
  }],
}
