#ifndef BONAVISTA_LOGGING_COLOR_PRINTF_H_
#define BONAVISTA_LOGGING_COLOR_PRINTF_H_

namespace logging {

enum Color {
  COLOR_RED,
  COLOR_GREEN
};

void ColorPrintf(Color color, const char* format, ...);

}  // namespace logging

#endif
