{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'memory_lib',
    'type': 'static_library',
    'sources': [
      'ref_count.cpp',
      'ref_count.h',
      'scoped_ptr.h',
      'scoped_ref.cpp',
      'scoped_ref.h',
    ],
  }, {
    'target_name': 'memory_test',
    'type': 'executable',
    'dependencies': [
      'memory_lib',
      '../testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'ref_count_test.cpp',
      'scoped_ptr_test.cpp',
      'scoped_ref_test.cpp',
      '../testing/test_main.cpp',
    ],
  }],
}
