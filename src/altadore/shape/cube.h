#ifndef ALTADORE_SHAPE_CUBE_H_
#define ALTADORE_SHAPE_CUBE_H_

#include "altadore/shape/shape.h"
#include "bonavista/util/macros.h"

class Cube : public Shape {
 public:
  Cube();
  virtual ~Cube();

  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const;
  virtual bool HasIntersection(const Ray& ray) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(Cube);
};

#endif
