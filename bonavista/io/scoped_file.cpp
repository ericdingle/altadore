#include "bonavista/io/scoped_file.h"

namespace io {

void FileCloseFunc::operator()(FILE* file) const {
  fclose(file);
}

}  // namespace io
