#ifndef SHAPE_SPHERE_H_
#define SHAPE_SPHERE_H_

#include <memory>
#include "shape/shape.h"

class Sphere : public Shape {
 public:
  static Result Create(
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<Invokable>* object);

  Sphere();
  Sphere(const Sphere&) = delete;
  Sphere& operator=(const Sphere&) = delete;
  virtual ~Sphere();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<const Variant>* var);

  virtual bool FindIntersection(const Ray& r, double* t, Point3* point, Vector3* normal) const;
  virtual bool HasIntersection(const Ray& ray, double max_t) const;
};

#endif  // SHAPE_SPHERE_H_
