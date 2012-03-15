#include "bonavista/memory/scoped_refptr.h"

#include "bonavista/memory/ref_count.h"

void AddRefFunc::operator()(const RefCount* ptr) const {
  if (ptr)
    ptr->AddRef();
}

void ReleaseFunc::operator()(const RefCount* ptr) const {
  if (ptr)
    ptr->Release();
}
