#ifndef BONAVISTA_TESTING_INSTANCE_COUNT_H_
#define BONAVISTA_TESTING_INSTANCE_COUNT_H_

#include "bonavista/base/macros.h"
#include "bonavista/base/types.h"

namespace testing {

class InstCount {
 public:
  InstCount();
  ~InstCount();

  static uint inst_count() { return inst_count_; }

 private:
  static uint inst_count_;

  DISALLOW_COPY_AND_ASSIGN(InstCount);
};

}  // namespace testing

#endif
