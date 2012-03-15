#include "bonavista/memory/ref_count.h"

#include "bonavista/base/macros.h"

RefCount::RefCount() : ref_count_(0) {
}

RefCount::~RefCount() {
}

void RefCount::AddRef() const {
  ++ref_count_;
}

void RefCount::Release() const {
  DCHECK(ref_count_ != 0);

  if (--ref_count_ == 0) {
    delete this;
  }
}
