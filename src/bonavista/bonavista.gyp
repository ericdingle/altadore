{
  'includes': [
    'common.gypi',
  ],
  'targets': [{
    'target_name': 'libs',
    'type': 'none',
    'dependencies': [
      'file/file.gyp:file_lib',
      'logging/logging.gyp:logging_lib',
      'memory/memory.gyp:memory_lib',
      'testing/testing.gyp:testing_lib',
    ],
  }, {
    'target_name': 'tests',
    'type': 'none',
    'dependencies': [
      'file/file.gyp:file_test',
      'logging/logging_test.gyp:logging_test',
      'memory/memory.gyp:memory_test',
      'testing/testing.gyp:testing_test',
    ],
  }],
}
