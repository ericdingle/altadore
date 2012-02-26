#ifndef ALTADORE_SHAPE_SHAPE_H_
#define ALTADORE_SHAPE_SHAPE_H_

#include "altadore/util/invokable.h"
#include "bonavista/util/macros.h"

class Point3;
class Ray;
class Vector3;

class Shape : public Invokable {
 public:
  Shape();
  virtual ~Shape();

  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const =0;
  virtual bool HasIntersection(const Ray& ray) const =0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Shape);
};

#endif
