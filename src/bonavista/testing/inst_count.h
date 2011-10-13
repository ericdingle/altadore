#ifndef BONAVISTA_TESTING_INSTANCE_COUNT_H_
#define BONAVISTA_TESTING_INSTANCE_COUNT_H_

#include "bonavista/types.h"

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
  InstCount(const InstCount&);
  void operator=(const InstCount&);

  static uint inst_count_;
};

template <typename T>
uint InstCount<T>::inst_count_ = 0;

}  // namespace testing

#endif
