#ifndef BONAVISTA_MEMORY_SCOPED_REF_H_
#define BONAVISTA_MEMORY_SCOPED_REF_H_

#include "bonavista/memory/scoped_ptr.h"

namespace memory {

class RefCount;

struct AddRefFunc {
  void operator()(const RefCount* ptr) const;
};

struct ReleaseFunc {
  void operator()(const RefCount* ptr) const;
};

template <typename T>
class scoped_ref : public scoped_ptr_base<T, AddRefFunc, ReleaseFunc> {
 public:
  scoped_ref(T* ptr=NULL) : scoped_ptr_base<T, AddRefFunc, ReleaseFunc>(ptr) {}
  ~scoped_ref() {}

  scoped_ref(const scoped_ref& r) : scoped_ptr_base<T, AddRefFunc, ReleaseFunc>(r.ptr()) {}
  template <typename U>
  scoped_ref(const scoped_ref<U>& r) : scoped_ptr_base<T, AddRefFunc, ReleaseFunc>(r.ptr()) {}

  scoped_ref& operator=(const scoped_ref& ref) {
    Reset(ref.ptr());
    return *this;
  }
};

}  // namespace memory

#endif
