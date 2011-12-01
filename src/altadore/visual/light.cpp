#include "altadore/visual/light.h"

Light::Light(const Point3* position, const Color* color) : position_(position), color_(color) {
  DASSERT(position != NULL);
  DASSERT(color != NULL);
}

Light::~Light() {
}
