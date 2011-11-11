#include "bonavista/testing/inst_count.h"

namespace testing {

uint InstCount::inst_count_ = 0;

InstCount::InstCount() {
  ++inst_count_;
}

InstCount::~InstCount() {
  --inst_count_;
}

}  // namespace testing
