#include "bonavista/file/scoped_file.h"

#include <stdio.h>

void FileCloseFunc::operator()(FILE* file) const {
  fclose(file);
}
