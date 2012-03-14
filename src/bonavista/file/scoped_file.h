#ifndef BONAVISTA_FILE_SCOPED_FILE_H_
#define BONAVISTA_FILE_SCOPED_FILE_H_

#include "bonavista/memory/scoped_ptr.h"

struct FileCloseFunc {
  void operator()(FILE* file) const;
};

typedef memory::scoped_ptr_base<FILE, memory::VoidFunc, FileCloseFunc> scoped_FILE;

#endif
