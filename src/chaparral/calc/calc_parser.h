#ifndef CHAPPARAL_CALC_CALC_PARSER_H_
#define CHAPPARAL_CALC_CALC_PARSER_H_

#include "chaparral/parse/parser.h"

class CalcParser : public Parser {
public:
  CalcParser();
  ~CalcParser();
private:
  CalcParser(const CalcParser& c);
  CalcParser& operator=(const CalcParser& c);
};

#endif
