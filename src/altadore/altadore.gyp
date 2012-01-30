{
  'includes': [
    '../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'altadore',
    'type': 'executable',
    'dependencies': [
      'ray_tracer/ray_tracer.gyp:ray_tracer_lib',
    ],
    'sources': [
      'main.cpp',
    ],
  }, {
    'target_name': 'libs',
    'type': 'none',
    'dependencies': [
      'algebra/algebra.gyp:algebra_lib',
      'image/image.gyp:image_lib',
      'ray_tracer/ray_tracer.gyp:ray_tracer_lib',
      'scene/scene.gyp:scene_lib',
      'shader/shader.gyp:shader_lib',
      'shape/shape.gyp:shape_lib',
    ],
  }, {
    'target_name': 'tests',
    'type': 'none',
    'dependencies': [
      'algebra/algebra.gyp:algebra_test',
      'image/image.gyp:image_test',
      'ray_tracer/ray_tracer.gyp:ray_tracer_test',
      'scene/scene.gyp:scene_test',
      'shader/shader.gyp:shader_test',
      'shape/shape.gyp:shape_test',
    ],
  }],
}
