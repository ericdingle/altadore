{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'string_lib',
    'type': 'static_library',
    'sources': [
      'format.cpp',
      'format.h',
    ],
  }, {
    'target_name': 'string_test',
    'type': 'executable',
    'dependencies': [
      'string_lib',
      '../testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'format_test.cpp',
      '../testing/test_main.cpp',
    ],
  }],
}
