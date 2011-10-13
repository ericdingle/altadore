{
  'includes': [
    '../../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'json',
    'type': 'executable',
    'dependencies': [
      '../parse/parse.gyp:parse_lib',
      '../../bonavista/io/io.gyp:io_lib',
    ],
    'sources': [
      'array_operator.cpp',
      'array_operator.h',
      'json_parser.cpp',
      'json_parser.h',
      'main.cpp',
      'object_operator.cpp',
      'object_operator.h',
    ],
  }],
}
