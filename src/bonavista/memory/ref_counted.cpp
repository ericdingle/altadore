#include "bonavista/memory/ref_counted.h"

#include "bonavista/base/macros.h"

RefCounted::RefCounted() : ref_count_(0) {
}

RefCounted::~RefCounted() {
}

void RefCounted::AddRef() const {
  ++ref_count_;
}

void RefCounted::Release() const {
  DCHECK(ref_count_ != 0);

  if (--ref_count_ == 0) {
    delete this;
  }
}
