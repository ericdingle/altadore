#ifndef CHAPARRAL_JSON_JSON_PARSER_H_
#define CHAPARRAL_JSON_JSON_PARSER_H_

#include "chaparral/parse/parser.h"

class JsonParser : public Parser {
public:
  JsonParser();
  ~JsonParser();

private:
  JsonParser(const JsonParser&);
  void operator=(const JsonParser&);
};

#endif
