#ifndef ALTADORE_ALGEBRA_POINT3_H_
#define ALTADORE_ALGEBRA_POINT3_H_

#include "bonavista/memory/ref_count.h"
#include "bonavista/util/types.h"

class Vector3;

class Point3 : public memory::RefCount {
public:
  Point3();
  Point3(double x, double y, double z);
  virtual ~Point3();

  Point3(const Point3& p);
  Point3& operator=(const Point3& p);

  double& operator[](uint i);
  double operator[](uint i) const;

  Point3 operator+(const Vector3& v) const;
  Vector3 operator-(const Point3& p) const;

  bool operator==(const Point3& p) const;

private:
  double p_[3];
};

#endif
