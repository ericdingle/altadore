#include "bonavista/memory/ref_count.h"

#include "bonavista/logging/assert.h"

namespace memory {

RefCount::RefCount() : ref_count_(0) {
}

RefCount::~RefCount() {
}

void RefCount::AddRef() const {
  ++ref_count_;
}

void RefCount::Release() const {
  DASSERT(ref_count_ != 0);

  if (--ref_count_ == 0) {
    delete this;
  }
}

}  // namespace memory
