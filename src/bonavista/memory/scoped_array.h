#ifndef BONAVISTA_MEMORY_SCOPED_ARRAY_H_
#define BONAVISTA_MEMORY_SCOPED_ARRAY_H_

#include "bonavista/base/macros.h"
#include "bonavista/memory/scoped_ptr.h"

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
  virtual ~scoped_array() {}

  T& operator[](uint index) {
    T* ptr = scoped_ptr_base<T, VoidFunc, DeleteArrayFunc<T> >::ptr();
    CHECK(ptr);
    return ptr[index];
  }

  const T& operator[](uint index) const {
    T* ptr = scoped_ptr_base<T, VoidFunc, DeleteArrayFunc<T> >::ptr();
    CHECK(ptr);
    return ptr[index];
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(scoped_array);
};

#endif
