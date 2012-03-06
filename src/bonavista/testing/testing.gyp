{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'testing_lib',
    'type': 'static_library',
    'dependencies': [
      '../logging/logging.gyp:logging_lib',
    ],
    'sources': [
      'inst_count.cpp',
      'inst_count.h',
      'test_case.cpp',
      'test_case.h',
      'test_case_expect.h',
    ],
  }, {
    'target_name': 'testing_test',
    'type': 'executable',
    'dependencies': [
      'testing_lib',
    ],
    'sources': [
      'inst_count_test.cpp',
      'test_case_test.cpp',
      'test_main.cpp',
    ],
  }],
}
