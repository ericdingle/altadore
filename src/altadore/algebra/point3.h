#ifndef ALTADORE_ALGEBRA_POINT3_H_
#define ALTADORE_ALGEBRA_POINT3_H_

#include <memory>
#include <string>
#include <vector>
#include "bonavista/base/types.h"
#include "chaparral/executer/invokable.h"

class Vector3;

class Point3 : public Invokable {
 public:
  static Result Create(
      const std::vector<std::shared_ptr<const Variant> >& args,
      std::shared_ptr<Invokable>* object);

  Point3();
  Point3(double x, double y, double z);
  virtual ~Point3();

  Point3(const Point3& p);
  Point3& operator=(const Point3& p);

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant> >& args,
      std::shared_ptr<const Variant>* var);

  double& operator[](uint i);
  double operator[](uint i) const;

  Point3 operator+(const Vector3& v) const;
  Vector3 operator-(const Point3& p) const;

  bool operator==(const Point3& p) const;

 private:
  double p_[3];
};

#endif
