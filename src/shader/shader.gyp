{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'shader_lib',
    'type': 'static_library',
    'dependencies': [
      '../algebra/algebra.gyp:algebra_lib',
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
      '../../bonavista/testing/testing.gyp:testing_main',
    ],
    'sources': [
      'color_test.cpp',
      'light_test.cpp',
      'light_vector_test.cpp',
      'material_test.cpp',
    ],
  }],
}
