#include "bonavista/io/io.h"

#include "bonavista/io/scoped_file.h"

namespace io {

FILE* OpenFile(const char* file_name, const char* mode) {
#if OS_WIN
  FILE* file;
  fopen_s(&file, file_name, mode);
  return file;
#elif OS_LINUX
  return fopen(file_name, mode);
#endif
}

bool ReadFile(const char* file_name, std::string* buffer) {
  scoped_FILE file(OpenFile(file_name, "r"));
  if (!file.ptr())
    return false;

  if (fseek(file.ptr(), 0, SEEK_END) != 0)
    return false;
  long size = ftell(file.ptr());
  if (size == -1)
    return false;
  rewind(file.ptr());

  buffer->resize(size);
  size_t usize = static_cast<size_t>(size);
  return fread(&buffer->at(0), 1, usize, file.ptr()) == usize;
}

}  // namespace io
