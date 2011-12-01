#include "bonavista/string/format.h"

#include <stdarg.h>
#include "bonavista/logging/assert.h"
#include "bonavista/memory/scoped_array.h"

namespace string {

std::string Format(const char* format, ...) {
  va_list args;
  va_start(args, format);

#if OS_WIN
  int len = _vscprintf(format, args);
#elif OS_LINUX
  va_list args_copy;
  va_copy(args_copy, args);
  int len = vsnprintf(NULL, 0, format, args_copy);
  va_end(args_copy);
#endif

  memory::scoped_array<char> buffer(new char[len + 1]);

#if OS_WIN
  int n = vsnprintf_s(buffer.ptr(), len + 1, len, format, args);
#elif OS_LINUX
  va_copy(args_copy, args);
  int n = vsnprintf(buffer.ptr(), len + 1, format, args);
  va_end(args_copy);
#endif

  DASSERT(n == len);

  va_end(args);
  return buffer.ptr();
}

}  // namespace string
