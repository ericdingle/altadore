{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'scene_interp_lib',
    'type': 'static_library',
    'dependencies': [
      '../algebra/algebra.gyp:algebra_lib',
      '../scene/scene.gyp:scene_lib',
      '../shader/shader.gyp:shader_lib',
      '../shape/shape.gyp:shape_lib',
      '../util/util.gyp:util_lib',
      '../../bonavista/string/string.gyp:string_lib',
      '../../chaparral/executer/executer.gyp:executer_lib',
      '../../chaparral/lexer/lexer.gyp:lexer_lib',
      '../../chaparral/parser/parser.gyp:parser_lib',
    ],
    'sources': [
      'scene_executer.cpp',
      'scene_executer.h',
      'scene_lexer.cpp',
      'scene_lexer.h',
      'scene_parser.cpp',
      'scene_parser.h',
    ],
  }, {
    'target_name': 'scene_interp_test',
    'type': 'executable',
    'dependencies': [
      'scene_interp_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'scene_executer_test.cpp',
      'scene_lexer_test.cpp',
      'scene_parser_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
