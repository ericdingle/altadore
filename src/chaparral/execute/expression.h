#ifndef CHAPARRAL_EXECUTE_EXPRESSION_H_
#define CHAPARRAL_EXECUTE_EXPRESSION_H_

#include "bonavista/memory/scoped_refptr.h"
#include "chaparral/value/value.h"

class Executer;

class Expression {
public:
  Expression();
  virtual ~Expression();

  virtual bool Execute(Executer* executer, memory::scoped_refptr<Value>* result) const=0;

private:
  Expression(const Expression&);
  void operator=(const Expression&);
};

#endif
