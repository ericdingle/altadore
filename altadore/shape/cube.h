#ifndef SHAPE_CUBE_H_
#define SHAPE_CUBE_H_

#include "altadore/shape/shape.h"

class Cube : public Shape {
 public:
  Cube() = default;
  Cube(const Cube&) = delete;
  Cube& operator=(const Cube&) = delete;
  ~Cube() override = default;

  bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const override;
  bool HasIntersection(const Ray& ray, double max_t) const override;
};

#endif  // SHAPE_CUBE_H_
