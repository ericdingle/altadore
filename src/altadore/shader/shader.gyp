{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'shader_lib',
    'type': 'static_library',
    'dependencies': [
      '../algebra/algebra.gyp:algebra_lib',
      '../util/util.gyp:util_lib',
      '../../bonavista/memory/memory.gyp:memory_lib',
      '../../chaparral/executer/executer.gyp:executer_lib',
    ],
    'sources': [
      'color.cpp',
      'color.h',
      'light.cpp',
      'light.h',
      'light_vector.cpp',
      'light_vector.h',
      'material.cpp',
      'material.h',
    ],
  }, {
    'target_name': 'shader_test',
    'type': 'executable',
    'dependencies': [
      'shader_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'color_test.cpp',
      'light_test.cpp',
      'light_vector_test.cpp',
      'material_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
