{
  'includes': [
    '../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'apps',
    'type': 'none',
    'dependencies': [
      'calc/calc.gyp:calc',
      'json/json.gyp:json',
    ],
  }, {
    'target_name': 'libs',
    'type': 'none',
    'dependencies': [
      'execute/execute.gyp:execute_lib',
      'parse/parse.gyp:parse_lib',
      'token/token.gyp:token_lib',
      'value/value.gyp:value_lib',
    ],
  }, {
    'target_name': 'tests',
    'type': 'none',
    'dependencies': [
      'execute/execute.gyp:execute_test',
      'parse/parse.gyp:parse_test',
      'token/token.gyp:token_test',
      'value/value.gyp:value_test',
    ],
  }],
}
