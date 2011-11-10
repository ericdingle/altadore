#ifndef BONAVISTA_IO_IO_H_
#define BONAVISTA_IO_IO_H_

#include <stdio.h>
#include <string>

namespace io {

FILE* OpenFile(const char* file_name, const char* mode);

bool ReadFile(const char* file_name, std::string* contents);

}  // namespace io

#endif
