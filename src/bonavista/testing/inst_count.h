#ifndef BONAVISTA_TESTING_INSTANCE_COUNT_H_
#define BONAVISTA_TESTING_INSTANCE_COUNT_H_

#include "bonavista/base/macros.h"
#include "bonavista/base/types.h"

namespace testing {

template <typename T>
class InstCount {
public:
  InstCount() {
    ++inst_count_;
  }
  virtual ~InstCount() {
    --inst_count_;
  }

  static uint inst_count() { return inst_count_; }

private:
  static uint inst_count_;

  DISALLOW_COPY_AND_ASSIGN(InstCount);
};

template <typename T>
uint InstCount<T>::inst_count_ = 0;

}  // namespace testing

#endif
