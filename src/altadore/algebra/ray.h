#ifndef ALTADORE_ALGEBRA_RAY_H_
#define ALTADORE_ALGEBRA_RAY_H_

#include "altadore/algebra/point3.h"
#include "altadore/algebra/vector3.h"

class Ray {
public:
  Ray(const Point3& origin, const Vector3& direction);
  ~Ray();

  const Point3& origin() const { return origin_; }
  const Vector3& direction() const { return direction_; }
  void set_direction(const Vector3& direction) { direction_ = direction; }

private:
  Ray(const Ray&);
  void operator=(const Ray&);

  const Point3 origin_;
  Vector3 direction_;
};

#endif
