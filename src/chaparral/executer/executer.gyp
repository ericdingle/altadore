{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'executer_lib',
    'type': 'static_library',
    'dependencies': [
      '../parser/parser.gyp:parser_lib',
      '../../bonavista/memory/memory.gyp:memory_lib',
    ],
    'sources': [
      'executer.cpp',
      'executer.h',
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
      'executer_test.cpp',
      'variant_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ]
  }],
}
