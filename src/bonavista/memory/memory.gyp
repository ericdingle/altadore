{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'memory_lib',
    'type': 'static_library',
    'sources': [
      'ref_counted.cpp',
      'ref_counted.h',
      'scoped_array.h',
      'scoped_ptr.h',
      'scoped_refptr.cpp',
      'scoped_refptr.h',
    ],
  }, {
    'target_name': 'memory_test',
    'type': 'executable',
    'dependencies': [
      'memory_lib',
      '../testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'ref_counted_test.cpp',
      'scoped_array_test.cpp',
      'scoped_ptr_test.cpp',
      'scoped_refptr_test.cpp',
      '../testing/test_main.cpp',
    ],
  }],
}
