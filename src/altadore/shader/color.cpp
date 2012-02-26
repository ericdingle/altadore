#include "altadore/shader/color.h"

#include <algorithm>
#include "bonavista/logging/assert.h"
#include "chaparral/executer/variant.h"

Invokable::Result Color::Create(
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    Invokable** object) {
  ASSERT(object);

  if (args.size() != 0 && args.size() != 3)
    return RESULT_ERR_ARG_SIZE;

  if (args.size() == 0) {
    memory::scoped_refptr<Color> color(new Color());
    *object = color.Release();
    return RESULT_OK;
  }

  double r;
  if (!args[0]->Get(&r))
    return RESULT_ERR_ARG_TYPE;
  double g;
  if (!args[1]->Get(&g))
    return RESULT_ERR_ARG_TYPE;
  double b;
  if (!args[2]->Get(&b))
    return RESULT_ERR_ARG_TYPE;

  memory::scoped_refptr<Color> color(new Color(r, g, b));
  *object = color.Release();
  return RESULT_OK;
}

Color::Color() : r_(0.0), g_(0.0), b_(0.0) {
}

Color::Color(double r, double g, double b) : r_(r), g_(g), b_(b) {
  DASSERT(r >= 0);
  DASSERT(g >= 0);
  DASSERT(b >= 0);
  Saturate();
}

Color::~Color() {
}

Color::Color(const Color& c) : r_(c.r_), g_(c.g_), b_(c.b_) {
}

Invokable::Result Color::Invoke(
    const std::string& name,
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    const Variant** var) {
  return RESULT_ERR_NAME;
}

Color Color::operator*(double d) const {
  DASSERT(d >= 0);
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
