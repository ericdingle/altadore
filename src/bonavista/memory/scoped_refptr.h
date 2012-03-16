#ifndef BONAVISTA_MEMORY_SCOPED_REFPTR_H_
#define BONAVISTA_MEMORY_SCOPED_REFPTR_H_

#include "bonavista/memory/scoped_ptr.h"

class RefCounted;

struct AddRefFunc {
  void operator()(const RefCounted* ptr) const;
};

struct ReleaseFunc {
  void operator()(const RefCounted* ptr) const;
};

template <typename T>
class scoped_refptr : public scoped_ptr_base<T, AddRefFunc, ReleaseFunc> {
 public:
  scoped_refptr(T* ptr=NULL) : scoped_ptr_base<T, AddRefFunc, ReleaseFunc>(ptr) {}
  virtual ~scoped_refptr() {}

  scoped_refptr(const scoped_refptr& r) : scoped_ptr_base<T, AddRefFunc, ReleaseFunc>(r.ptr()) {}
  template <typename U>
  scoped_refptr(const scoped_refptr<U>& r) : scoped_ptr_base<T, AddRefFunc, ReleaseFunc>(r.ptr()) {}

  scoped_refptr& operator=(const scoped_refptr& ref) {
    Reset(ref.ptr());
    return *this;
  }
};

#endif
