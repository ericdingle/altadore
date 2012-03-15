#ifndef BONAVISTA_MEMORY_REF_COUNT_H_
#define BONAVISTA_MEMORY_REF_COUNT_H_

#include "bonavista/base/macros.h"
#include "bonavista/base/types.h"

class RefCount {
 public:
  RefCount();
  virtual ~RefCount();

  void AddRef() const;
  void Release() const;

  uint ref_count() const { return ref_count_; }

 private:
  mutable uint ref_count_;

  DISALLOW_COPY_AND_ASSIGN(RefCount);
};

#endif
