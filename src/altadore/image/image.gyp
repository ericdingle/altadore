{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'image_lib',
    'type': 'static_library',
    'dependencies': [
      '../../bonavista/io/io.gyp:io_lib',
    ],
    'sources': [
      'bitmap.cpp',
      'bitmap.h',
    ],
  }, {
    'target_name': 'image_test',
    'type': 'executable',
    'dependencies': [
      'image_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'bitmap_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ]
  }],
}
