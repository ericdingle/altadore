{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'logging_test',
    'type': 'executable',
    'dependencies': [
      'logging.gyp:logging_lib',
      '../testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'string_format_test.cpp',
      '../testing/test_main.cpp',
    ],
  }],
}
