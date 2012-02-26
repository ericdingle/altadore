#ifndef ALTADORE_SHAPE_SPHERE_H_
#define ALTADORE_SHAPE_SPHERE_H_

#include "altadore/shape/shape.h"
#include "altadore/util/invokable.h"
#include "bonavista/util/macros.h"

class Sphere : public Invokable, public Shape {
 public:
  static Result Create(
      const std::vector<memory::scoped_refptr<const Variant> >& args,
      Invokable** object);

  Sphere();
  virtual ~Sphere();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<memory::scoped_refptr<const Variant> >& args,
      const Variant** var);

  virtual bool FindIntersection(const Ray& r, double* t, Point3* point, Vector3* normal) const;
  virtual bool HasIntersection(const Ray& ray) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(Sphere);
};

#endif
