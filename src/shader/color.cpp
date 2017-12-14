#include "shader/color.h"

#include <algorithm>

Color::Color() : r_(0.0), g_(0.0), b_(0.0) {
}

Color::Color(double r, double g, double b) : r_(r), g_(g), b_(b) {
  Saturate();
}

Color::Color(const Color& c) : r_(c.r_), g_(c.g_), b_(c.b_) {
}

Color Color::operator*(double d) const {
  return Color(r_ * d, g_ * d, b_ * d);
}

Color& Color::operator+=(double d) {
  r_ += d;
  g_ += d;
  b_ += d;
  Saturate();
  return *this;
}

Color& Color::operator+=(const Color& c) {
  r_ += c.r_;
  g_ += c.g_;
  b_ += c.b_;
  Saturate();
  return *this;
}

Color& Color::operator*=(const Color& c) {
  r_ *= c.r_;
  g_ *= c.g_;
  b_ *= c.b_;
  return *this;
}

void Color::Saturate() {
  r_ = std::min(r_, 1.0);
  g_ = std::min(g_, 1.0);
  b_ = std::min(b_, 1.0);
}

bool Color::operator==(const Color& c) const {
  return r_ == c.r_ && g_ == c.g_ && b_ == c.b_;
}
