{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'executer_lib',
    'type': 'static_library',
    'sources': [
      'variant.cpp',
      'variant.h',
    ],
  }, {
    'target_name': 'executer_test',
    'type': 'executable',
    'dependencies': [
      'executer_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'variant_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ]
  }],
}
