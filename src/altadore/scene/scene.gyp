{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'scene_lib',
    'type': 'static_library',
    'dependencies': [
      '../shader/shader.gyp:shader_lib',
      '../shape/shape.gyp:shape_lib',
    ],
    'sources': [
      'scene_node.cpp',
      'scene_node.h',
      'shape_node.cpp',
      'shape_node.h',
      'transform_node.cpp',
      'transform_node.h',
    ],
  }, {
    'target_name': 'scene_test',
    'type': 'executable',
    'dependencies': [
      'scene_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
    ],
    'sources': [
      'shape_node_test.cpp',
      'transform_node_test.cpp',
      '../../bonavista/testing/test_main.cpp',
    ],
  }],
}
