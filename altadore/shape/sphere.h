#ifndef SHAPE_SPHERE_H_
#define SHAPE_SPHERE_H_

#include "altadore/shape/shape.h"

class Sphere : public Shape {
 public:
  Sphere() = default;
  Sphere(const Sphere&) = delete;
  Sphere& operator=(const Sphere&) = delete;
  ~Sphere() override = default;

  bool FindIntersection(const Ray& r, double* t, Point3* point, Vector3* normal) const override;
  bool HasIntersection(const Ray& ray, double max_t) const override;
};

#endif  // SHAPE_SPHERE_H_
