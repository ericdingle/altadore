#ifndef BONAVISTA_FILE_SCOPED_FILE_H_
#define BONAVISTA_FILE_SCOPED_FILE_H_

#include "bonavista/memory/scoped_ptr.h"

struct FileCloseFunc {
  void operator()(FILE* file) const;
};

typedef scoped_ptr_base<FILE, VoidFunc, FileCloseFunc> scoped_FILE;

#endif
