{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'logging_lib',
    'type': 'static_library',
    'sources': [
      'assert.h',
      'color_printf.cpp',
      'color_printf.h',
    ],
  }],
}
