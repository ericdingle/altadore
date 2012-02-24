{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'algebra_lib',
    'type': 'static_library',
    'dependencies': [
      '../util/util.gyp:util_lib',
      '../../bonavista/memory/memory.gyp:memory_lib',
      '../../chaparral/executer/executer.gyp:executer_lib',
    ],
    'sources': [
      'algebra_constants.cpp',
      'algebra_constants.h',
      'matrix4.cpp',
      'matrix4.h',
      'point3.cpp',
      'point3.h',
      'ray.cpp',
      'ray.h',
      'vector3.cpp',
      'vector3.h',
    ],
  }, {
    'target_name': 'algebra_test',
    'type': 'executable',
    'dependencies': [
      'algebra_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'matrix4_test.cpp',
      'point3_test.cpp',
      'ray_test.cpp',
      'vector3_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
