#ifndef SHAPE_SHAPE_H_
#define SHAPE_SHAPE_H_

#include "third_party/chaparral/src/executer/invokable.h"

class Point3;
class Ray;
class Vector3;

class Shape : public Invokable {
 public:
  Shape();
  Shape(const Shape&) = delete;
  Shape& operator=(const Shape&) = delete;
  virtual ~Shape();

  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const = 0;
  virtual bool HasIntersection(const Ray& ray, double max_t) const = 0;
};

#endif  // SHAPE_SHAPE_H_
