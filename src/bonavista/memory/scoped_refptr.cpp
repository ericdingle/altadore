#include "bonavista/memory/scoped_refptr.h"

#include "bonavista/memory/ref_counted.h"

void AddRefFunc::operator()(const RefCounted* ptr) const {
  if (ptr)
    ptr->AddRef();
}

void ReleaseFunc::operator()(const RefCounted* ptr) const {
  if (ptr)
    ptr->Release();
}
