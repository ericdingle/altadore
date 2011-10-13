{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'token_lib',
    'type': 'static_library',
    'sources': [
      'token.cpp',
      'token.h',
      'token_stream.cpp',
      'token_stream.h',
    ],
  }, {
    'target_name': 'token_test',
    'type': 'executable',
    'dependencies': [
      'token_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'token_test.cpp',
      'token_stream_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
