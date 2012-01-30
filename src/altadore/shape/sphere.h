#ifndef ALTADORE_SHAPE_SPHERE_H_
#define ALTADORE_SHAPE_SPHERE_H_

#include "altadore/shape/shape.h"
#include "bonavista/util/macros.h"

class Sphere : public Shape {
 public:
  Sphere();
  virtual ~Sphere();

  virtual bool FindIntersection(const Ray& r, double* t, Point3* point, Vector3* normal) const;
  virtual bool HasIntersection(const Ray& ray) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(Sphere);
};

#endif
