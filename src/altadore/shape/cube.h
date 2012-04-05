#ifndef ALTADORE_SHAPE_CUBE_H_
#define ALTADORE_SHAPE_CUBE_H_

#include "altadore/shape/shape.h"
#include "bonavista/base/macros.h"

class Cube : public Shape {
 public:
  static Result Create(
      const std::vector<scoped_refptr<const Variant> >& args,
      Invokable** object);

  Cube();
  virtual ~Cube();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var);

  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const;
  virtual bool HasIntersection(const Ray& ray, double max_t) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(Cube);
};

#endif
