#ifndef ALTADORE_SHADER_COLOR_H_
#define ALTADORE_SHADER_COLOR_H_

#include "bonavista/memory/ref_count.h"
#include "bonavista/util/types.h"

class Color : public memory::RefCount {
 public:
  Color();
  Color(double r, double g, double b);
  ~Color();

  Color(const Color&);

  Color operator*(double d) const;
  Color& operator+=(double d);
  Color& operator+=(const Color& c);
  Color& operator*=(const Color& c);

  bool operator==(const Color& c) const;

  double r() const { return r_; }
  double g() const { return g_; }
  double b() const { return b_; }

 private:
  void Saturate();

  double r_;
  double g_;
  double b_;

  void operator=(const Color&);
};

#endif