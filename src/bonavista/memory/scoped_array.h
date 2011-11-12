#ifndef BONAVISTA_MEMORY_SCOPED_ARRAY_H_
#define BONAVISTA_MEMORY_SCOPED_ARRAY_H_

#include "bonavista/memory/scoped_ptr.h"

namespace memory {

template <typename T>
struct DeleteArrayFunc {
  void operator()(T* ptr) const {
    delete [] ptr;
  }
};

template <typename T>
class scoped_array : public scoped_ptr_base<T, VoidFunc, DeleteArrayFunc<T> > {
 public:
  explicit scoped_array(T* ptr=NULL) : scoped_ptr_base<T, VoidFunc, DeleteArrayFunc<T> >(ptr) {}
  ~scoped_array() {}

  T& operator[](uint index) {
    T* ptr = scoped_ptr_base<T, VoidFunc, DeleteArrayFunc<T> >::ptr();
    DASSERT(ptr != NULL);
    return ptr[index];
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(scoped_array);
};

}  // namespace memory

#endif
