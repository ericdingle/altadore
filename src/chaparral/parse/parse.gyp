{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'parse_lib',
    'type': 'static_library',
    'dependencies': [
      '../token/token.gyp:token_lib',
      '../../bonavista/memory/memory.gyp:memory_lib',
      '../../bonavista/string/string.gyp:string_lib',
    ],
    'sources': [
      'identifier_symbol.cpp',
      'identifier_symbol.h',
      'infix_symbol.cpp',
      'infix_symbol.h',
      'number_symbol.cpp',
      'number_symbol.h',
      'parser.cpp',
      'parser.h',
      'prefix_symbol.cpp',
      'prefix_symbol.h',
      'string_symbol.cpp',
      'string_symbol.h',
      'symbol.cpp',
      'symbol.h'
    ],
  }, {
    'target_name': 'parse_test',
    'type': 'executable',
    'dependencies': [
      'parse_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'identifier_symbol_test.cpp',
      'number_symbol_test.cpp',
      'parser_test.cpp',
      'string_symbol_test.cpp',
      'symbol_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
