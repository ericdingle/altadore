#include "altadore/shader/light.h"

Light::Light(const Point3* position, const Color* color) : position_(position), color_(color) {
  ASSERT(position);
  ASSERT(color);
}

Light::~Light() {
}
