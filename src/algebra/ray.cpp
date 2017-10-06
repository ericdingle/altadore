#include "ray.h"

Ray::Ray(const Point3& origin, const Vector3& direction) : origin_(origin), direction_(direction) {
}

Ray::~Ray() {
}
