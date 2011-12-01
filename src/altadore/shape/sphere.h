#ifndef ALTADORE_SHAPE_SPHERE_H_
#define ALTADORE_SHAPE_SPHERE_H_

#include "altadore/shape/shape.h"

class Sphere : public Shape {
public:
  Sphere();
  ~Sphere();

  bool FindIntersection(const Ray& r, double* t, Point3* point, Vector3* normal) const;
  bool HasIntersection(const Ray& ray) const;

private:
  Sphere(const Sphere&);
  void operator=(const Sphere& s);
};

#endif
