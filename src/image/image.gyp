{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'image_lib',
    'type': 'static_library',
    'dependencies': [
      '../../bonavista/file/file.gyp:file_lib',
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
      '../../bonavista/testing/testing.gyp:testing_main',
    ],
    'sources': [
      'bitmap_test.cpp',
    ]
  }],
}
