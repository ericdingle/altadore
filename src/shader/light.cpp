#include "shader/light.h"

Light::Light(const std::shared_ptr<const Point3>& position,
             const std::shared_ptr<const Color>& color)
    : position_(position), color_(color) {
}
