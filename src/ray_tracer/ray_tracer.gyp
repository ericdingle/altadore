{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'ray_tracer_lib',
    'type': 'static_library',
    'dependencies': [
      '../image/image.gyp:image_lib',
      '../scene/scene.gyp:scene_lib',
      '../shader/shader.gyp:shader_lib',
    ],
    'sources': [
      'ray_tracer.cpp',
      'ray_tracer.h',
      'ray_tracer_constants.cpp',
      'ray_tracer_constants.h',
    ],
  }, {
    'target_name': 'ray_tracer_test',
    'type': 'executable',
    'dependencies': [
      'ray_tracer_lib',
      '../../bonavista/testing/testing.gyp:testing_lib',
      '../../bonavista/testing/testing.gyp:testing_main',
    ],
    'sources': [
      'ray_tracer_test.cpp',
    ],
  }],
}
