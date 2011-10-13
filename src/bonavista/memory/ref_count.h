#ifndef BONAVISTA_MEMORY_REF_COUNT_H_
#define BONAVISTA_MEMORY_REF_COUNT_H_

#include "bonavista/base/types.h"

namespace memory {

class RefCount {
public:
  RefCount();
  virtual ~RefCount();

  void AddRef() const;
  void Release() const;

  uint ref_count() const { return ref_count_; }

private:
  RefCount(const RefCount&);
  void operator=(const RefCount&);

  mutable uint ref_count_;
};

}  // namespace memory

#endif
