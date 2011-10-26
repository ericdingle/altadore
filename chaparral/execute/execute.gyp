{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'execute_lib',
    'type': 'static_library',
    'dependencies': [
      '../parse/parse.gyp:parse_lib',
      '../value/value.gyp:value_lib',
      '../../bonavista/memory/memory.gyp:memory_lib',
      '../../bonavista/string/string.gyp:string_lib',
    ],
    'sources': [
      'executer.cpp',
      'executer.h',
      'expression.cpp',
      'expression.h',
    ]
  }, {
    'target_name': 'execute_test',
    'type': 'executable',
    'dependencies': [
      'execute_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'executer_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ]
  }]
}
