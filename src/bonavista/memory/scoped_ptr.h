#ifndef BONAVISTA_MEMORY_SCOPED_PTR_H_
#define BONAVISTA_MEMORY_SCOPED_PTR_H_

#include "bonavista/base/macros.h"
#include "bonavista/base/types.h"
#include "bonavista/logging/assert.h"

namespace memory {

template <typename T, typename InitFunc, typename UninitFunc>
class scoped_ptr_base {
 public:
  explicit scoped_ptr_base(T* ptr=NULL) : ptr_(ptr) {
    init_func_(ptr_);
  }
  virtual ~scoped_ptr_base() {
    uninit_func_(ptr_);
  }

  T* operator->() const {
    DASSERT(ptr_ != NULL);
    return ptr_;
  }

  void Reset(T* ptr) {
    uninit_func_(ptr_);
    ptr_ = ptr;
    init_func_(ptr);
  }

  T* Release() {
    T* ptr = ptr_;
    ptr_ = NULL;
    return ptr;
  }

  T** Receive() {
    Reset(NULL);
    return &ptr_;
  }

  T* ptr() const { return ptr_; }

 private:
  static const InitFunc init_func_;
  static const UninitFunc uninit_func_;

  T* ptr_;

  DISALLOW_COPY_AND_ASSIGN(scoped_ptr_base);
};

template <typename T, typename InitFunc, typename UninitFunc>
const InitFunc scoped_ptr_base<T, InitFunc, UninitFunc>::init_func_ = InitFunc();

template <typename T, typename InitFunc, typename UninitFunc>
const UninitFunc scoped_ptr_base<T, InitFunc, UninitFunc>::uninit_func_ = UninitFunc();

struct VoidFunc {
  void operator()(const void* v) const {}
};

template <typename T>
struct DeletePtrFunc {
  void operator()(T* ptr) const {
    delete ptr;
  }
};

template <typename T>
class scoped_ptr : public scoped_ptr_base<T, VoidFunc, DeletePtrFunc<T> > {
 public:
  explicit scoped_ptr(T* ptr=NULL) : scoped_ptr_base<T, VoidFunc, DeletePtrFunc<T> >(ptr) {}
  ~scoped_ptr() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(scoped_ptr);
};

}  // namespace memory

#endif
