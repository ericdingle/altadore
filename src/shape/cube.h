#ifndef SHAPE_CUBE_H_
#define SHAPE_CUBE_H_

#include <memory>
#include "shape/shape.h"

class Cube : public Shape {
 public:
  static Result Create(
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<Invokable>* object);

  Cube();
  Cube(const Cube&) = delete;
  Cube& operator=(const Cube&) = delete;
  virtual ~Cube();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<const Variant>* var);

  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const;
  virtual bool HasIntersection(const Ray& ray, double max_t) const;
};

#endif  // SHAPE_CUBE_H_
