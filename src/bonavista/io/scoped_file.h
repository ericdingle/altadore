#ifndef BONAVISTA_IO_SCOPED_FILE_H_
#define BONAVISTA_IO_SCOPED_FILE_H_

#include "bonavista/memory/scoped_ptr.h"

namespace io {

struct FileCloseFunc {
  void operator()(FILE* file) const;
};

typedef memory::scoped_ptr_base<FILE, memory::VoidFunc, FileCloseFunc> scoped_FILE;

}  // namespace io

#endif
