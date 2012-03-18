{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'shape_lib',
    'type': 'static_library',
    'dependencies': [
      '../algebra/algebra.gyp:algebra_lib',
      '../../bonavista/memory/memory.gyp:memory_lib',
    ],
    'sources': [
      'cube.cpp',
      'cube.h',
      'shape.cpp',
      'shape.h',
      'shape_constants.cpp',
      'shape_constants.h',
      'sphere.cpp',
      'sphere.h',
    ],
  }, {
    'target_name': 'shape_test',
    'type': 'executable',
    'dependencies': [
      'shape_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
      '../../bonavista/testing/testing.gyp:testing_main',
    ],
    'sources': [
      'cube_test.cpp',
      'sphere_test.cpp',
    ],
  }],
}
