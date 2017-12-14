#ifndef SHADER_COLOR_H_
#define SHADER_COLOR_H_

#include <memory>

class Color {
 public:
  Color();
  Color(double r, double g, double b);
  ~Color() = default;

  Color(const Color&);
  Color& operator=(const Color&) = delete;

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
};

#endif  // SHADER_COLOR_H_
