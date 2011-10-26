#ifndef BONAVISTA_LOGGING_COLOR_PRINTF_H_
#define BONAVISTA_LOGGING_COLOR_PRINTF_H_

namespace logging {

enum Color {
  DEFAULT,
  RED,
  GREEN
};

void ColorPrintf(Color color, const char* format, ...);

}  // namespace logging

#endif
