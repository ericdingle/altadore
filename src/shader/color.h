#ifndef SHADER_COLOR_H_
#define SHADER_COLOR_H_

#include <memory>
#include "third_party/chaparral/src/executer/invokable.h"

class Color : public Invokable {
 public:
  static Result Create(
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<Invokable>* object);

  Color();
  Color(double r, double g, double b);
  virtual ~Color();

  Color(const Color&);

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<const Variant>* var);

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

#endif  // SHADER_COLOR_H_
