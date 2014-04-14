{
  'includes': [
    '../bonavista/common.gypi',
  ],
  'targets': [{
    'target_name': 'bins',
    'type': 'none',
    'dependencies': [
      'calc/calc.gyp:calc',
      'json/json.gyp:json',
    ],
  }, {
    'target_name': 'libs',
    'type': 'none',
    'dependencies': [
      'calc/calc.gyp:calc_lib',
      'executer/executer.gyp:executer_lib',
      'json/json.gyp:json_lib',
      'lexer/lexer.gyp:lexer_lib',
      'parser/parser.gyp:parser_lib',
    ],
  }, {
    'target_name': 'tests',
    'type': 'none',
    'dependencies': [
      'calc/calc.gyp:calc_test',
      'executer/executer.gyp:executer_test',
      'json/json.gyp:json_test',
      'lexer/lexer.gyp:lexer_test',
      'parser/parser.gyp:parser_test',
    ],
  }],
}
