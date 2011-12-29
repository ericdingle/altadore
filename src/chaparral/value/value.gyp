{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'value_lib',
    'type': 'static_library',
    'dependencies': [
      '../../bonavista/memory/memory.gyp:memory_lib',
      '../../bonavista/string/string.gyp:string_lib',
    ],
    'sources': [
      'boolean_value.cpp',
      'boolean_value.h',
      'map_value.cpp',
      'map_value.h',
      'number_value.cpp',
      'number_value.h',
      'string_value.cpp',
      'string_value.h',
      'value.cpp',
      'value.h', 
    ],
  }, {
    'target_name': 'value_test',
    'type': 'executable',
    'dependencies': [
      'value_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'boolean_value_test.cpp',
      'map_value_test.cpp',
      'number_value_test.cpp',
      'string_value_test.cpp',
      'value_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
