#include "altadore/shape/sphere.h"

#include <math.h>
#include "altadore/algebra/ray.h"
#include "altadore/shape/shape_constants.h"
#include "bonavista/logging/assert.h"
#include "chaparral/executer/variant.h"

namespace {

void GetCoefficients(const Ray& ray, double* a, double* b, double* c) {
  ASSERT(a);
  ASSERT(b);
  ASSERT(c);

  const Vector3& d = ray.direction();
  const Point3& o = ray.origin();

  *a = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
  *b = 2*(d[0]*o[0] + d[1]*o[1] + d[2]*o[2]);
  *c = o[0]*o[0] + o[1]*o[1] + o[2]*o[2] - 1;
}

int GetRoots(double a, double b, double c, double roots[2]) {
  if (a == 0.0) {
    if (b == 0.0) {
      return 0;
    } else {
      roots[0] = -c/b;
      return 1;
    }
  } else {
    double d = b*b - 4*a*c;
    if (d < 0) {
      return 0;
    } else if (d == 0.0) {
      roots[0] = -b / (2*a);
      return 1;
    } else {
      double q = sqrt(d);
      roots[0] = (-b + q) / (2*a);
      roots[1] = (-b - q) / (2*a);
      return 2;
    }
  }
}

}  // namespace

Invokable::Result Sphere::Create(
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    Invokable** object) {
  ASSERT(object);

  if (args.size() != 0)
    return RESULT_ERR_ARG_SIZE;

  memory::scoped_refptr<Sphere> sphere(new Sphere());
  *object = sphere.Release();
  return RESULT_OK;
}

Sphere::Sphere() {
}

Sphere::~Sphere() {
}

Invokable::Result Sphere::Invoke(
    const std::string& name,
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    const Variant** var) {
  return RESULT_ERR_NAME;
}

bool Sphere::FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const {
  ASSERT(point);
  ASSERT(normal);

  double a, b, c;
  GetCoefficients(ray, &a, &b, &c);

  double roots[2];
  int num_roots = GetRoots(a, b, c, roots);

  if (num_roots == 0) {
    return false;
  } else if (num_roots == 1) {
    if (roots[0] >= kEpsilon)
      *t = roots[0];
    else
      return false;
  } else if (num_roots == 2) {
    if (roots[0] > roots[1]) {
      double x = roots[0];
      roots[0] = roots[1];
      roots[1] = x;
    }
    if (roots[0] >= kEpsilon)
      *t = roots[0];
    else if (roots[1] >= kEpsilon)
      *t = roots[1];
    else
      return false;
  }

  *point = ray.origin() + ray.direction() * *t;
  *normal = *point;
  return true;
}

bool Sphere::HasIntersection(const Ray& ray) const {
  const Point3& origin = ray.origin();
  const Vector3& direction = ray.direction();

  // Find the point on the line closest to the sphere's origin.
  double t = -direction.Dot(Vector3(origin)) / direction.Dot(direction);
  if (t < kEpsilon)
    return false;

  // Use Vector3's dot method to calculate the squared length.
  Vector3 point(origin + direction * t);
  return point.Dot(point) <= 1;
}
