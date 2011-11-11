#include "bonavista/logging/color_printf.h"

#include <stdarg.h>
#include <stdio.h>
#if OS_WIN
#include <windows.h>
#endif

namespace logging {

#if OS_WIN
WORD GetColorAttribute(Color color) {
  switch (color) {
    case COLOR_RED:
      return FOREGROUND_RED;
    case COLOR_GREEN:
      return FOREGROUND_GREEN;
    default:
      return 0;
  }
}
#elif OS_LINUX
const char* GetAnsiColorCode(Color color) {
  switch (color) {
    case COLOR_RED:
      return "1";
    case COLOR_GREEN:
      return "2";
    default:
      return NULL;
  };
}
#endif

void ColorPrintf(Color color, const char* format, ...) {
  va_list args;
  va_start(args, format);
#if OS_WIN
  const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_SCREEN_BUFFER_INFO buffer_info;
  GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
  const WORD old_color_attrs = buffer_info.wAttributes;

  SetConsoleTextAttribute(stdout_handle, GetColorAttribute(color));
  vprintf(format, args);
  SetConsoleTextAttribute(stdout_handle, old_color_attrs);
#elif OS_LINUX
  printf("\033[0;3%sm", GetAnsiColorCode(color));
  vprintf(format, args);
  printf("\033[m");
#endif

  va_end(args);
}

}  // namespace logging
