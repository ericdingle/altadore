#include "bonavista/memory/scoped_ref.h"

#include "bonavista/memory/ref_count.h"

namespace memory {

void AddRefFunc::operator()(const RefCount* ptr) const {
  if (ptr)
    ptr->AddRef();
}

void ReleaseFunc::operator()(const RefCount* ptr) const {
  if (ptr)
    ptr->Release();
}

}  // namespace memory
