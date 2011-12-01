#ifndef ALTADORE_SHAPE_CUBE_H_
#define ALTADORE_SHAPE_CUBE_H_

#include "altadore/shape/shape.h"

class Cube : public Shape {
public:
  Cube();
  ~Cube();

  bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const;
  bool HasIntersection(const Ray& ray) const;

private:
  Cube(const Cube&);
  void operator=(const Cube&);
};

#endif
