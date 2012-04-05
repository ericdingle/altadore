#ifndef ALTADORE_SHAPE_SPHERE_H_
#define ALTADORE_SHAPE_SPHERE_H_

#include "altadore/shape/shape.h"
#include "bonavista/base/macros.h"

class Sphere : public Shape {
 public:
  static Result Create(
      const std::vector<scoped_refptr<const Variant> >& args,
      Invokable** object);

  Sphere();
  virtual ~Sphere();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var);

  virtual bool FindIntersection(const Ray& r, double* t, Point3* point, Vector3* normal) const;
  virtual bool HasIntersection(const Ray& ray, double max_t) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(Sphere);
};

#endif
