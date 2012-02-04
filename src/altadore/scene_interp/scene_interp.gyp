{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'scene_interp_lib',
    'type': 'static_library',
    'dependencies': [
      '../../bonavista/string/string.gyp:string_lib',
      '../../chaparral/lexer/lexer.gyp:lexer_lib',
    ],
    'sources': [
      'scene_lexer.cpp',
      'scene_lexer.h',
    ],
  }, {
    'target_name': 'scene_interp_test',
    'type': 'executable',
    'dependencies': [
      'scene_interp_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'scene_lexer_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
