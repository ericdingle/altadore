{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'util_lib',
    'type': 'static_library',
    'dependencies': [
      '../../bonavista/memory/memory.gyp:memory_lib',
    ],
    'sources': [
      'invokable.cpp',
      'invokable.h',
    ],
  }],
}
