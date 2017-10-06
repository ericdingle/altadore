#include "shader/color.h"

#include <algorithm>
#include "third_party/chaparral/src/executer/variant.h"

Invokable::Result Color::Create(
    const std::vector<std::shared_ptr<const Variant>>& args,
    std::shared_ptr<Invokable>* object) {
  DCHECK(object);

  if (args.size() != 0 && args.size() != 3)
    return RESULT_ERR_ARG_SIZE;

  std::shared_ptr<Color> color;
  if (args.size() == 0) {
    color.reset(new Color());
  } else {
    double r, g, b;
    if (!args[0]->Get(&r) || !args[1]->Get(&g) || !args[2]->Get(&b))
      return RESULT_ERR_ARG_TYPE;
    color.reset(new Color(r, g, b));
  }

  *object = color;
  return RESULT_OK;
}

Color::Color() : r_(0.0), g_(0.0), b_(0.0) {
}

Color::Color(double r, double g, double b) : r_(r), g_(g), b_(b) {
  DCHECK(r >= 0);
  DCHECK(g >= 0);
  DCHECK(b >= 0);
  Saturate();
}

Color::~Color() {
}

Color::Color(const Color& c) : r_(c.r_), g_(c.g_), b_(c.b_) {
}

Invokable::Result Color::Invoke(
    const std::string& name,
    const std::vector<std::shared_ptr<const Variant>>& args,
    std::shared_ptr<const Variant>* var) {
  return RESULT_ERR_NAME;
}

Color Color::operator*(double d) const {
  DCHECK(d >= 0);
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
