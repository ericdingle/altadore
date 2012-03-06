{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'io_lib',
    'type': 'static_library',
    'sources': [
      'io.cpp',
      'io.h',
      'scoped_file.cpp',
      'scoped_file.h',
    ],
  }, {
    'target_name': 'io_test',
    'type': 'executable',
    'dependencies': [
      'io_lib',
      '../testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'io_test.cpp',
      'scoped_file_test.cpp',
      '../testing/test_main.cpp',
    ],
  }]
}
