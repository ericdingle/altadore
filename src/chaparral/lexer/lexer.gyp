{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'lexer_lib',
    'type': 'static_library',
    'sources': [
      'lexer.cpp',
      'lexer.h',
      'token.cpp',
      'token.h',
      'token_stream.cpp',
      'token_stream.h',
    ],
  }, {
    'target_name': 'lexer_test',
    'type': 'executable',
    'dependencies': [
      'lexer_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'token_stream_test.cpp',
      'token_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ]
  }],
}
