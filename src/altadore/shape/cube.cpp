#include "altadore/shape/cube.h"

#include <limits>
#include "altadore/algebra/ray.h"
#include "altadore/shape/shape_constants.h"
#include "bonavista/logging/assert.h"

Invokable::Result Cube::Create(
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    Invokable** object) {
  ASSERT(object);

  if (args.size() != 0)
    return RESULT_ERR_ARG_SIZE;

  memory::scoped_refptr<Cube> cube(new Cube());
  *object = cube.Release();
  return RESULT_OK;
}

Cube::Cube() {
}

Cube::~Cube() {
}

Invokable::Result Cube::Invoke(
    const std::string& name,
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    const Variant** var) {
  return RESULT_ERR_NAME;
}

bool Cube::FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const {
  ASSERT(t);
  ASSERT(point);
  ASSERT(normal);

  const Point3& origin = ray.origin();
  const Vector3& direction = ray.direction();

  *t = std::numeric_limits<double>::max();

  for (int i = 0; i < 3; ++i) {
    if (direction[i] == 0)  // Ray is parallel to plane.
      continue;

    for (int j = -1; j <= 1; j += 2) {
      if (direction[i] * j > 0)  // Ray hits the back of the plane.
        continue;

      double s = (j - origin[i]) / direction[i];
      if (s >= kEpsilon && s < *t) {
        uint k = (i + 1) % 3;
        double y = origin[k] + direction[k] * s;

        uint l = (i + 2) % 3;
        double z = origin[l] + direction[l] * s;

        if (y >= -1 && y <= 1 && z >= -1 && z <= 1) {
          *t = s;
          *point = origin + direction * s;
          *normal = Vector3();
          (*normal)[i] = j;
        }
      }
    }
  }

  return *t != std::numeric_limits<double>::max();
}

bool Cube::HasIntersection(const Ray& ray) const {
  const Point3& origin = ray.origin();
  const Vector3& direction = ray.direction();

  for (int i = 0; i < 3; ++i) {
    for (int j = -1; j <= 1; j += 2) {
      double t = (j - origin[i]) / direction[i];
      if (t >= kEpsilon) {
        uint k = (i + 1) % 3;
        double y = origin[k] + direction[k] * t;

        uint l = (i + 2) % 3;
        double z = origin[l] + direction[l] * t;

        if (y >= -1 && y <= 1 && z >= -1 && z <= 1)
          return true;
      }
    }
  }

  return false;
}
