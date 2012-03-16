#ifndef ALTADORE_UTIL_INVOKABLE_H_
#define ALTADORE_UTIL_INVOKABLE_H_

#include <string>
#include <vector>
#include "bonavista/memory/ref_counted.h"
#include "bonavista/memory/scoped_refptr.h"

class Variant;

class Invokable : public RefCounted {
 public:
  enum Result {
    RESULT_OK,
    RESULT_ERR_NAME,
    RESULT_ERR_ARG_SIZE,
    RESULT_ERR_ARG_TYPE,
    RESULT_ERR_FAIL
  };

  Invokable();
  virtual ~Invokable();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var) =0;
};

#endif
