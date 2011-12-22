{
  'includes': [
    '../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'exes',
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
      'json/json.gyp:json_lib',
      'lexer/lexer.gyp:lexer_lib',
      'parse/parse.gyp:parse_lib',
      'parser/parser.gyp:parser_lib',
      'token/token.gyp:token_lib',
      'value/value.gyp:value_lib',
    ],
  }, {
    'target_name': 'tests',
    'type': 'none',
    'dependencies': [
      'execute/execute.gyp:execute_test',
      'json/json.gyp:json_test',
      'lexer/lexer.gyp:lexer_test',
      'parse/parse.gyp:parse_test',
      'parser/parser.gyp:parser_test',
      'token/token.gyp:token_test',
      'value/value.gyp:value_test',
    ],
  }],
}
